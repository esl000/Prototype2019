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

APrototypeProjectCharacter::APrototypeProjectCharacter()
{
	// Set size for player capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->bAbsoluteRotation = true; // Don't want arm to rotate when character does
	CameraBoom->TargetArmLength = 800.f;
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

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> BodyMesh(TEXT("SkeletalMesh'/Game/ParagonGreystone/Characters/Heroes/Greystone/Skins/Novaborn/Meshes/Greystone_Novaborn.Greystone_Novaborn'"));
	if (BodyMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(BodyMesh.Object);
	}


	CursorToWorld->DecalSize = FVector(16.0f, 32.0f, 32.0f);
	CursorToWorld->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f).Quaternion());

	CollisionCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CollsionSphere"));
	CollisionCapsule->SetCapsuleSize(10.f, 60.f);
	CollisionCapsule->AttachToComponent(GetMesh(),
		FAttachmentTransformRules(EAttachmentRule::KeepRelative, false),
		TEXT("FX_Sword_Bottom"));
	CollisionCapsule->SetRelativeLocation(FVector(0.f, 0.f, 60.f));
	CollisionCapsule->SetGenerateOverlapEvents(true);
	CollisionCapsule->OnComponentBeginOverlap.AddDynamic(this, &APrototypeProjectCharacter::OnHitCollision);
	CollisionCapsule->SetGenerateOverlapEvents(false);

	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;


	PushingPower = 2000.f;

	LookDirection = FVector::ForwardVector;
	DestLookDirection = FVector::ForwardVector;
	FocusYaw = 0.f;
	SpineRotationYaw = 0.f;

	CurrentState = EAnimationState::E_IDLE;

	CurrentAttackCount = 0;
	IgnoreAttackAnim = false;
	bUnlockSkill = false;

	WarkSpeed = 600.0f;
	DashSpeed = 1800.f;
	DashAcceletor = 1400.f;

	AttackCoolTime = 0.3f;
	ChargeCoolTime = 3.f;
	SkillCoolTime = 10.f;
	DashCoolTime = 3.f;

	CurrentAttackCoolTime = 0.f;
	CurrentChargeCoolTime = 0.f;
	CurrentSkillCoolTime = 0.f;
	CurrentDashCoolTime = 0.f;
}

void APrototypeProjectCharacter::BeginPlay()
{
	Super::BeginPlay();
	InGameUIInstance = CreateWidget<UInGameUI>(Cast<APlayerController>(GetController()), InGameUIClass);
	InGameUIInstance->Player = this;
	InGameUIInstance->AddToViewport();
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
}

void APrototypeProjectCharacter::ColliderCheck(bool toggle)
{
	CollisionCapsule->SetGenerateOverlapEvents(toggle);
}

void APrototypeProjectCharacter::Attack()
{
	TArray<FHitResult> result;
	FCollisionQueryParams params;
	FCollisionShape shape;
	shape.ShapeType = ECollisionShape::Type::Box;
	shape.Box.HalfExtentX = 50.f;
	shape.Box.HalfExtentY = 50.f;
	shape.Box.HalfExtentZ = 50.f;
	params.AddIgnoredActor(this);



	if (GetWorld()->SweepMultiByChannel(result, GetActorLocation(), GetActorLocation() + 200.f * GetActorForwardVector(), FQuat::Identity,
		ECollisionChannel::ECC_Pawn, shape, params))
	{
		for (int i = 0; i < result.Num(); ++i)
		{
			ABotGame* character = Cast<ABotGame>(result[i].GetActor());
			if (character == nullptr)
				return;

			if(character->Stat.Stack < character->Stat.MaxStack)
				character->Stat.Stack++;
			FVector particleDir = (character->GetActorLocation() - GetActorLocation()).GetSafeNormal2D();

			ApplyCameraShake(0.3f);

			GetWorld()->SpawnActor<AActor>(Particle, 
				result[i].ImpactPoint,
				particleDir.ToOrientationRotator());
		}
	}
}

void APrototypeProjectCharacter::Charge()
{
	TArray<FHitResult> result;
	FCollisionQueryParams params;
	FCollisionShape shape;
	shape.ShapeType = ECollisionShape::Type::Box;
	shape.Box.HalfExtentX = 50.f;
	shape.Box.HalfExtentY = 50.f;
	shape.Box.HalfExtentZ = 50.f;
	params.AddIgnoredActor(this);



	if (GetWorld()->SweepMultiByChannel(result, GetActorLocation(), GetActorLocation() + 200.f * GetActorForwardVector(), FQuat::Identity,
		ECollisionChannel::ECC_Pawn, shape, params))
	{
		for (int i = 0; i < result.Num(); ++i)
		{
			ABotGame* character = Cast<ABotGame>(result[i].GetActor());
			if (character == nullptr)
				return;

			UCharacterMovementComponent* movement = Cast<UCharacterMovementComponent>(character->GetComponentByClass(UCharacterMovementComponent::StaticClass()));
			if (movement == nullptr)
				return;

			FVector particleDir = (character->GetActorLocation() - GetActorLocation()).GetSafeNormal2D();
			movement->StopActiveMovement();
			movement->Velocity = particleDir * PushingPower * character->Stat.Stack;
			character->Stat.Stack = 0;
			character->Stat.IsMovable = false;

			ApplyCameraShake(1.f);

			GetWorld()->SpawnActor<AActor>(Particle,
				result[i].ImpactPoint,
				particleDir.ToOrientationRotator());
		}
	}
}

void APrototypeProjectCharacter::UltimateSkill()
{
}

void APrototypeProjectCharacter::Dash()
{
	CurrentDashSpeed = DashSpeed;
	DashDirection = DestLookDirection;
	GetCharacterMovement()->MaxWalkSpeed = CurrentDashSpeed;
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

	//UE_LOG(LogPrototypeProject, Warning, TEXT("HIT"));

	if (ABotGame* hitCharacter = Cast<ABotGame>(OtherActor))
	{
		if (CurrentState == EAnimationState::E_ATTACK)
		{
			hitCharacter->Stat.Stack < hitCharacter->Stat.MaxStack ? hitCharacter->Stat.Stack++ : hitCharacter->Stat.MaxStack;
		}
		else if (CurrentState == EAnimationState::E_CHARGING)
		{
			FVector particleDir = (hitCharacter->GetActorLocation() - GetActorLocation()).GetSafeNormal2D();
			hitCharacter->GetCharacterMovement()->Velocity = particleDir * 1000.f * hitCharacter->Stat.Stack;
			hitCharacter->Stat.Stack = 0;
			hitCharacter->CurrentState = EAnimationState1::E_HIT;
		}
		else if(CurrentState == EAnimationState::E_SKILL)
		{

		}
	}

}
