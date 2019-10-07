// Fill out your copyright notice in the Description page of Project Settings.
#include "BotGame.h"
#include "AAIController.h"
#include "PrototypeProjectCharacter.h"
#include "PrototypeProject.h"
//#include "Components/CapsuleComponent.h"



ABotGame::ABotGame()
{
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> BodyMesh(TEXT("SkeletalMesh'/Game/ParagonGreystone/Characters/Heroes/Greystone/Skins/Novaborn/Meshes/Greystone_Novaborn.Greystone_Novaborn'"));
	if (BodyMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(BodyMesh.Object);
	}

	GetCharacterMovement()->SetMovementMode(MOVE_NavWalking);

	CollisionCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CollsionSphere"));
	CollisionCapsule->SetCapsuleSize(10.f, 60.f);
	CollisionCapsule->AttachToComponent(GetMesh(),
		FAttachmentTransformRules(EAttachmentRule::KeepRelative, false),
		TEXT("FX_Sword_Bottom"));
	CollisionCapsule->SetRelativeLocation(FVector(0.f, 0.f, 60.f));
	CollisionCapsule->SetGenerateOverlapEvents(true);
	CollisionCapsule->OnComponentBeginOverlap.AddDynamic(this, &ABotGame::OnHitCollision);
	CollisionCapsule->SetGenerateOverlapEvents(false);
	bUseControllerRotationYaw = true;
	DuringAttack = false;
	AIControllerClass = AAAIController::StaticClass();
}

void ABotGame::NotifyActorBeginOverlap(AActor * OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
}

void ABotGame::FaceRotation(FRotator NewControlRotation, float DeltaTime)
{
	FRotator CurrentRotation = FMath::RInterpTo(GetActorRotation(), NewControlRotation, DeltaTime, 8.0f);
	Super::FaceRotation(CurrentRotation, DeltaTime);
}

void ABotGame::BeginPlay()
{
	Super::BeginPlay();
}

void ABotGame::PlayMeleeAnim()
{
	if (APrototypeProjectCharacter* character = Cast<APrototypeProjectCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn()))
	{
		if(character->Stat.Stack > character->Stat.MaxStack * 0.6f)
			CurrentState = EAnimationState1::E_CHARGING;
		else
			CurrentState = EAnimationState1::E_ATTACK;
	}
	//if (!DuringAttack)
	//{
	//	DuringAttack = true;
	//	float AnimDuration = PlayAnimMontage(MeleeAnim);
	//}
	//float AnimDuration = PlayAnimMontage(MeleeAnim);

	//GetWorldTimerManager().SetTimer(TimerHandle_EnemyStopAttack, this, &ABotGame::StopMeleeAnim, 1, false);
}

void ABotGame::StopMeleeAnim()
{
	/*if (DuringAttack)
	{
		DuringAttack = false;
		StopAnimMontage(MeleeAnim);
	}
	StopAnimMontage(MeleeAnim);*/
}

void ABotGame::OnHitCollision(UPrimitiveComponent * OverlappedComp, 
	AActor * OtherActor, UPrimitiveComponent * OtherComp, 
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	/*UE_LOG(LogPrototypeProject, Warning, TEXT("%s"), *OverlappedComp->GetName());*/
	if (OverlappedComp->GetCollisionObjectType() != CollisionCapsule->GetCollisionObjectType())
		return;

	//UE_LOG(LogPrototypeProject, Warning, TEXT("HIT"));

	if (APrototypeProjectCharacter* hitCharacter = Cast<APrototypeProjectCharacter>(OtherActor))
	{
		if (CurrentState == EAnimationState1::E_ATTACK)
		{
			hitCharacter->Stat.Stack < hitCharacter->Stat.MaxStack ? hitCharacter->Stat.Stack++ : hitCharacter->Stat.MaxStack;
		}
		else if (CurrentState == EAnimationState1::E_CHARGING)
		{
			FVector particleDir = (hitCharacter->GetActorLocation() - GetActorLocation()).GetSafeNormal2D();
			hitCharacter->GetCharacterMovement()->Velocity = particleDir * 1000.f * hitCharacter->Stat.Stack;
			hitCharacter->Stat.Stack = 0;
			hitCharacter->CurrentState = EAnimationState::E_HIT;
		}
	}
}

void ABotGame::Attack()
{
	CollisionCapsule->SetGenerateOverlapEvents(true);
}

void ABotGame::Charge()
{
	CollisionCapsule->SetGenerateOverlapEvents(true);

}

void ABotGame::EndAttack()
{
	CollisionCapsule->SetGenerateOverlapEvents(false);
}

void ABotGame::EndCharge()
{
	CollisionCapsule->SetGenerateOverlapEvents(false);
}

void ABotGame::Tick(float DeltaTime)
{
	// AI가 장외로 떨어지면 카운트 ++ 위치 : MyActor.cpp
	if (GetActorLocation().Z <= -200.0f)
	{
		Destroy(this);
		for (TActorIterator<AActor> It(GetWorld()); It; ++It)
		{
			AActor* Actor = *It;
			if (Actor->GetName() == "MyActor_1")
			{
				Actor->Tick(1);
			}
		}
	}
	Super::Tick(DeltaTime);
}
