// Fill out your copyright notice in the Description page of Project Settings.
#include "BotGame.h"
#include "AAIController.h"
#include "PrototypeProjectCharacter.h"
#include "PrototypeProject.h"



ABotGame::ABotGame()
{

	GetCharacterMovement()->SetMovementMode(MOVE_NavWalking);
	bUseControllerRotationYaw = true;
	DuringAttack = false;
	AIControllerClass = AAAIController::StaticClass();

	Stat.PushingPower = 2000.f;
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
			CurrentState = EAnimationState::E_CHARGING;
		else
			CurrentState = EAnimationState::E_ATTACK;
	}
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
		if (CurrentState == EAnimationState::E_ATTACK)
		{
			hitCharacter->Stat.Stack < hitCharacter->Stat.MaxStack ? hitCharacter->Stat.Stack++ : hitCharacter->Stat.MaxStack;
			hitCharacter->ApplyCameraShake(0.15f);
			PlayEffect(hitCharacter);
		}
		else if (CurrentState == EAnimationState::E_CHARGING)
		{
			if (hitCharacter->CurrentState == EAnimationState::E_HIT)
				return;

			FVector particleDir = (hitCharacter->GetActorLocation() - GetActorLocation()).GetSafeNormal2D();
			hitCharacter->GetCharacterMovement()->Velocity = particleDir * Stat.PushingPower * hitCharacter->Stat.Stack;
			hitCharacter->Stat.Stack = 0;
			hitCharacter->CurrentState = EAnimationState::E_HIT;
			hitCharacter->ApplyCameraShake(0.35f);
			PlayEffect(hitCharacter);
		}
	}
}

void ABotGame::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// AI가 장외로 떨어지면 카운트 ++ 위치 : MyActor.cpp
	if (GetActorLocation().Z <= -200.0f)
	{
		Destroy(this);
	}
}
