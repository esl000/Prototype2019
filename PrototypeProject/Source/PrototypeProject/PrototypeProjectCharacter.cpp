// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "PrototypeProjectCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Materials/Material.h"
#include "Engine/World.h"
#include "PrototypeProject.h"
#include "DummyAICharacter.h"
#include "BotGame.h"
#include "Blueprint/UserWidget.h"
#include "InGameUI.h"
#include "LevelManager.h"

APrototypeProjectCharacter::APrototypeProjectCharacter()
{
	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	RootComponent->bAbsoluteRotation = true;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->bAbsoluteRotation = true; // Don't want arm to rotate when character does
	CameraBoom->TargetArmLength = 200.f;
	CameraBoom->RelativeRotation = FRotator(-60.f, 0.f, 0.f);
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	// Create a camera...
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Create a decal in the world to show the cursor's location
	CursorToWorld = CreateDefaultSubobject<UDecalComponent>("CursorToWorld");
	CursorToWorld->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UMaterial> DecalMaterialAsset(TEXT("Material'/Game/TopDownCPP/Blueprints/M_Cursor_Decal.M_Cursor_Decal'"));
	if (DecalMaterialAsset.Succeeded())
	{
		CursorToWorld->SetDecalMaterial(DecalMaterialAsset.Object);
	}

	CursorToWorld->DecalSize = FVector(4.0f, 8.0f, 8.0f);
	CursorToWorld->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f).Quaternion());

	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	LookDirection = FVector::ForwardVector;
	DestLookDirection = FVector::ForwardVector;
	FocusYaw = 0.f;
	SpineRotationYaw = 0.f;

	CurrentState = EAnimationState::E_IDLE;

	CurrentAttackCount = 0;
	bUnlockSkill = false;
	bUnlockDash = false;

	WarkSpeed = GetCharacterMovement()->MaxWalkSpeed;
	DashSpeed = WarkSpeed * 3.f;
	DashAcceletor = WarkSpeed * 2.f;

	AttackCoolTime = 0.3f;
	ChargeCoolTime = 3.f;
	SkillCoolTime = 10.f;
	DashCoolTime = 3.f;

	CurrentAttackCoolTime = 0.f;
	CurrentChargeCoolTime = 0.f;
	CurrentSkillCoolTime = 0.f;
	CurrentDashCoolTime = 0.f;

	AccelatorSkillPushingPower = 1.5f;
	Stat.PushingPower = 5000.f;
	Stat.Damage = 3.f;
}

void APrototypeProjectCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void APrototypeProjectCharacter::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

	if (CursorToWorld != nullptr)
	{
		if (UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayEnabled())
		{
			if (UWorld* World = GetWorld())
			{
				FHitResult HitResult;
				FCollisionQueryParams Params(NAME_None, FCollisionQueryParams::GetUnknownStatId());
				FVector StartLocation = TopDownCameraComponent->GetComponentLocation();
				FVector EndLocation = TopDownCameraComponent->GetComponentRotation().Vector() * 2000.0f;
				Params.AddIgnoredActor(this);
				World->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility, Params);
				FQuat SurfaceRotation = HitResult.ImpactNormal.ToOrientationRotator().Quaternion();
				CursorToWorld->SetWorldLocationAndRotation(HitResult.Location, SurfaceRotation);
				DestLookDirection = (HitResult.Location - GetActorLocation()).GetSafeNormal2D();
			}
		}
		else if (APlayerController* PC = Cast<APlayerController>(GetController()))
		{
			FHitResult TraceHitResult;
			PC->GetHitResultUnderCursor(ECC_Visibility, true, TraceHitResult);
			FVector CursorFV = TraceHitResult.ImpactNormal;
			FRotator CursorR = CursorFV.Rotation();
			CursorToWorld->SetWorldLocation(TraceHitResult.Location);
			CursorToWorld->SetWorldRotation(CursorR);
			DestLookDirection = (TraceHitResult.Location - GetActorLocation()).GetSafeNormal2D();
		}
	}


	if (CurrentState != EAnimationState::E_DASH)
	{
		RotateSight();
	}
	else
	{
		FocusYaw = 0.f;
		SpineRotationYaw = 0.f;

		CurrentDashSpeed -= DashAcceletor * DeltaSeconds;
		GetCharacterMovement()->MaxWalkSpeed = CurrentDashSpeed;
		AddMovementInput(DashDirection, 1.f);
	}

	if (CurrentAttackCoolTime > 0.f)
	{
		CurrentAttackCoolTime -= DeltaSeconds;
		CurrentAttackCoolTime = CurrentAttackCoolTime < 0.f ? 0.f : CurrentAttackCoolTime;
	}

	if (CurrentChargeCoolTime > 0.f)
	{
		CurrentChargeCoolTime -= DeltaSeconds;
		CurrentChargeCoolTime = CurrentChargeCoolTime < 0.f ? 0.f : CurrentChargeCoolTime;
	}

	if (CurrentSkillCoolTime > 0.f)
	{
		CurrentSkillCoolTime -= DeltaSeconds;
		CurrentSkillCoolTime = CurrentSkillCoolTime < 0.f ? 0.f : CurrentSkillCoolTime;
	}

	if (CurrentDashCoolTime > 0.f)
	{
		CurrentDashCoolTime -= DeltaSeconds;
		CurrentDashCoolTime = CurrentDashCoolTime < 0.f ? 0.f : CurrentDashCoolTime;
	}

	if (GetActorLocation().Z <= -200.0f)
	{
		Die();
		return;
	}

	if (GetController() != nullptr && (CurrentState == EAnimationState::E_ATTACK ||
		CurrentState == EAnimationState::E_CHARGING ||
		CurrentState == EAnimationState::E_SKILL))
	{
		GetController()->SetControlRotation(DestLookDirection.ToOrientationRotator());
	}
	else
	{
		GetController()->SetControlRotation(GetVelocity().ToOrientationRotator());
	}
}

void APrototypeProjectCharacter::ToggleInputComponent(bool isEnable)
{
	if (!InputComponent.IsValid())
		return;

	if (APlayerController* pc = Cast<APlayerController>(GetController()))
	{
		if(isEnable)
			pc->PushInputComponent(InputComponent.Get());
		else
			pc->PopInputComponent(InputComponent.Get());
	}
}

void APrototypeProjectCharacter::CheckEndAttack(int count)
{
	if (count < CurrentAttackCount)
		return;

	CurrentAttackCoolTime = AttackCoolTime;
	CurrentAttackCount = 0;
	EndMotion();
}

void APrototypeProjectCharacter::UltimateSkill()
{
	if (CurrentState == EAnimationState::E_HIT
		|| CurrentState == EAnimationState::E_ATTACK
		|| CurrentState == EAnimationState::E_DASH
		|| CurrentState == EAnimationState::E_CHARGING)
		return;

	if (CurrentSkillCoolTime > 0.00001f)
		return;

	if (CurrentState != EAnimationState::E_SKILL)
		SetActorRotation(DestLookDirection.ToOrientationRotator());

	CurrentSkillCoolTime = SkillCoolTime;
	CurrentState = EAnimationState::E_SKILL;
}

void APrototypeProjectCharacter::Dash()
{
	CurrentDashSpeed = DashSpeed;
	DashDirection = GetActorForwardVector();
	GetCharacterMovement()->MaxWalkSpeed = CurrentDashSpeed;
	GetCharacterMovement()->Velocity = DashDirection * CurrentDashSpeed * GetWorld()->DeltaTimeSeconds;
	GetCapsuleComponent()->SetGenerateOverlapEvents(false);
}

void APrototypeProjectCharacter::EndDash()
{
	GetCharacterMovement()->MaxWalkSpeed = WarkSpeed;
	GetCapsuleComponent()->SetGenerateOverlapEvents(true);
	CurrentState = EAnimationState::E_IDLE;
}

void APrototypeProjectCharacter::RotateSight()
{
	float focusYaw = LookDirection.ToOrientationRotator().Yaw;
	float RotationYaw = GetActorRotation().Yaw;

	if (focusYaw < 0.f)
		focusYaw += 360.f;

	if (focusYaw < RotationYaw)
		focusYaw += 360.f;

	if (focusYaw > RotationYaw + 180.f)
	{
		focusYaw = focusYaw - 360.f;
	}

	focusYaw -= RotationYaw;


	if (RotationYaw < 0.f)
		RotationYaw += 360.f;



	float destYaw = DestLookDirection.ToOrientationRotator().Yaw;

	if (destYaw < 0.f)
		destYaw += 360.f;

	if (destYaw < RotationYaw)
		destYaw += 360.f;

	if (destYaw > RotationYaw + 180.f)
	{
		destYaw = destYaw - 360.f;
	}

	destYaw -= RotationYaw;



	focusYaw = FMath::Lerp(focusYaw, destYaw, 3.f * GetWorld()->GetDeltaSeconds());
	float worldDestYaw = focusYaw + RotationYaw;
	LookDirection = FRotator(0.f, worldDestYaw, 0.f).Vector();

	if (FMath::Abs(focusYaw) > 90.f)
	{
		FocusYaw = (focusYaw / FMath::Abs(focusYaw)) * 90.f;
		SpineRotationYaw = (focusYaw / FMath::Abs(focusYaw)) * (FMath::Abs(focusYaw) - 90.f);
	}
	else
	{
		FocusYaw = focusYaw;
		SpineRotationYaw = 0.f;
	}
}

void APrototypeProjectCharacter::OnHitCollision(UPrimitiveComponent * OverlappedComp, 
	AActor * OtherActor, UPrimitiveComponent * OtherComp, 
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OverlappedComp->GetCollisionObjectType() != CollisionCapsule->GetCollisionObjectType())
		return;

	if (ABotGame* hitCharacter = Cast<ABotGame>(OtherActor))
	{
		if (CurrentState == EAnimationState::E_ATTACK)
		{
			hitCharacter->Stat.Stack < hitCharacter->Stat.MaxStack ? hitCharacter->Stat.Stack++ : hitCharacter->Stat.MaxStack;
			hitCharacter->Damage(Stat.Damage);
			ApplyCameraShake(0.3f);
			PlayEffect(hitCharacter);
		}
		else if (CurrentState == EAnimationState::E_CHARGING)
		{
			if (hitCharacter->CurrentState == EAnimationState::E_HIT)
				return;
			hitCharacter->Damage(Stat.Damage * 1.5f);
			hitCharacter->GetCharacterMovement()->StopActiveMovement();
			FVector particleDir = (hitCharacter->GetActorLocation() - GetActorLocation()).GetSafeNormal2D();
			hitCharacter->GetCharacterMovement()->Velocity = particleDir * Stat.PushingPower * hitCharacter->Stat.Stack;
			hitCharacter->Stat.Stack = 0;
			hitCharacter->CurrentState = EAnimationState::E_HIT;
			ApplyCameraShake(0.7f);
			PlayEffect(hitCharacter);
		}
		else if(CurrentState == EAnimationState::E_SKILL)
		{
			if (hitCharacter->CurrentState == EAnimationState::E_HIT)
				return;
			hitCharacter->Damage(Stat.Damage * 2.f);
			hitCharacter->GetCharacterMovement()->StopActiveMovement();
			FVector particleDir = (hitCharacter->GetActorLocation() - GetActorLocation()).GetSafeNormal2D();
			hitCharacter->GetCharacterMovement()->Velocity = particleDir * AccelatorSkillPushingPower * Stat.PushingPower * hitCharacter->Stat.Stack;
			hitCharacter->Stat.Stack = 0;
			hitCharacter->CurrentState = EAnimationState::E_HIT;
			ApplyCameraShake(1.f);
			PlayEffect(hitCharacter);
		}
	}

}

void APrototypeProjectCharacter::SetupPlayerInputComponent(UInputComponent * PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	InputComponent = PlayerInputComponent;

	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &APrototypeProjectCharacter::BasicAttack);
	PlayerInputComponent->BindAction("Charging", IE_Pressed, this, &APrototypeProjectCharacter::ChargingAttack);
	PlayerInputComponent->BindAction("Dash", IE_Pressed, this, &APrototypeProjectCharacter::StartDash);
	PlayerInputComponent->BindAction("Skill", IE_Pressed, this, &APrototypeProjectCharacter::UseSkill);
	PlayerInputComponent->BindAxis("MoveForward", this, &APrototypeProjectCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APrototypeProjectCharacter::MoveRight);
}

void APrototypeProjectCharacter::PossessedBy(AController * newController)
{
	if (APlayerController* pc = Cast<APlayerController>(newController))
	{
		if (InGameUIInstance != nullptr)
			InGameUIInstance->AddToViewport();
		else
		{
			InGameUIInstance = CreateWidget<UInGameUI>(pc, InGameUIClass);
			InGameUIInstance->Player = this;
			InGameUIInstance->AddToViewport();
		}

		InGameUIInstance->Show(ALevelManager::GetInstance()->CurrentStage);
		GetCharacterMovement()->GravityScale = 1.f;
		Stat.Hp = Stat.MaxHp;
		Stat.Stack = 0;

		CurrentAttackCount = 0.f;
		CurrentChargeCoolTime = 0.f;
		CurrentDashCoolTime = 0.f;
		CurrentSkillCoolTime = 0.f;
		SetActorTickEnabled(true);
	}
}

void APrototypeProjectCharacter::UnPossessed()
{
	Super::UnPossessed();
	GetCharacterMovement()->GravityScale = 0.f;
	SetActorTickEnabled(false);
	if (InGameUIInstance != nullptr)
		InGameUIInstance->RemoveFromViewport();
}

void APrototypeProjectCharacter::Die()
{
	SetActorLocation(FVector::ZeroVector);
	ALevelManager::GetInstance()->PrevRetryGame();
}

void APrototypeProjectCharacter::StartDash()
{
	if (CurrentState != EAnimationState::E_HIT && CurrentDashCoolTime < 0.00001f)
	{
		CurrentState = EAnimationState::E_DASH;
		CurrentDashSpeed = DashSpeed;
		DashDirection = GetActorForwardVector();
		GetCharacterMovement()->MaxWalkSpeed = CurrentDashSpeed;
		GetCapsuleComponent()->SetGenerateOverlapEvents(false);
		CurrentDashCoolTime = DashCoolTime;
	}
}

void APrototypeProjectCharacter::MoveForward(float delta)
{
	if (CurrentState == EAnimationState::E_IDLE
		|| CurrentState == EAnimationState::E_MOVE)
	{
		GetCharacterMovement()->AddInputVector(FVector(delta, 0.f, 0.f));
		CurrentState = EAnimationState::E_MOVE;
	}
}

void APrototypeProjectCharacter::MoveRight(float delta)
{
	if (CurrentState == EAnimationState::E_IDLE
		|| CurrentState == EAnimationState::E_MOVE)
	{
		GetCharacterMovement()->AddInputVector(FVector(0.f, delta, 0.f));
		CurrentState = EAnimationState::E_MOVE;
	}
}

void APrototypeProjectCharacter::BasicAttack()
{
	if (CurrentState == EAnimationState::E_HIT
		|| CurrentState == EAnimationState::E_CHARGING
		|| CurrentState == EAnimationState::E_DASH
		|| CurrentState == EAnimationState::E_SKILL)
		return;

	if (CurrentAttackCoolTime > 0.00001f)
		return;

	if (CurrentState != EAnimationState::E_ATTACK)
	{
		//SetActorRotation(DestLookDirection.ToOrientationRotator());
		CurrentAttackCount = 1;
	}
	else
	{
		if (CurrentAttackCount < 3)
			CurrentAttackCount++;				
	}

	CurrentState = EAnimationState::E_ATTACK;
}

void APrototypeProjectCharacter::ChargingAttack()
{
	if (CurrentState == EAnimationState::E_HIT
		|| CurrentState == EAnimationState::E_ATTACK
		|| CurrentState == EAnimationState::E_DASH
		|| CurrentState == EAnimationState::E_SKILL)
		return;

	if (CurrentChargeCoolTime > 0.00001f)
		return;

	//if (CurrentState != EAnimationState::E_CHARGING)
		//SetActorRotation(DestLookDirection.ToOrientationRotator());

	CurrentChargeCoolTime = ChargeCoolTime;
	CurrentState = EAnimationState::E_CHARGING;
}

void APrototypeProjectCharacter::UseSkill()
{
	if (CurrentState == EAnimationState::E_HIT
		|| CurrentState == EAnimationState::E_ATTACK
		|| CurrentState == EAnimationState::E_DASH
		|| CurrentState == EAnimationState::E_SKILL)
		return;

	if (CurrentSkillCoolTime > 0.00001f)
		return;

	//if (CurrentState != EAnimationState::E_SKILL)
		//SetActorRotation(DestLookDirection.ToOrientationRotator());

	CurrentSkillCoolTime = SkillCoolTime;
	CurrentState = EAnimationState::E_SKILL;
}

void APrototypeProjectCharacter::Hit()
{
	ApplyCameraShake(1.f);
	GetCharacterMovement()->MaxWalkSpeed = WarkSpeed;
	CurrentState = EAnimationState::E_HIT;
}
