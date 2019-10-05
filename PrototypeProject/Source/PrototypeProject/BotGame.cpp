// Fill out your copyright notice in the Description page of Project Settings.
#include "BotGame.h"
#include "AAIController.h"



ABotGame::ABotGame()
{
	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollsionSphere"));
	CollisionSphere->InitSphereRadius(10.0f);
	CollisionSphere->AttachTo(GetMesh(), "WeaponPoint");
	bUseControllerRotationYaw = true;
	DuringAttack = false;
	AIControllerClass = AAAIController::StaticClass();
}


void ABotGame::NotifyActorBeginOverlap(AActor * OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	if (OtherActor->IsA(ABotGame::StaticClass()) && DuringAttack == true)
	{
		UGameplayStatics::ApplyDamage(OtherActor, 10.0f, NULL, this, UDamageType::StaticClass());
	}
}

void ABotGame::FaceRotation(FRotator NewControlRotation, float DeltaTime)
{
	FRotator CurrentRotation = FMath::RInterpTo(GetActorRotation(), NewControlRotation, DeltaTime, 8.0f);
	Super::FaceRotation(CurrentRotation, DeltaTime);
}

void ABotGame::PlayMeleeAnim()
{
	if (!DuringAttack)
	{
		DuringAttack = true;
		float AnimDuration = PlayAnimMontage(MeleeAnim);
	}
	float AnimDuration = PlayAnimMontage(MeleeAnim);

	GetWorldTimerManager().SetTimer(TimerHandle_EnemyStopAttack, this, &ABotGame::StopMeleeAnim, 1, false);
}

void ABotGame::StopMeleeAnim()
{
	if (DuringAttack)
	{
		DuringAttack = false;
		StopAnimMontage(MeleeAnim);
	}
	StopAnimMontage(MeleeAnim);
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
