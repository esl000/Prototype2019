// Fill out your copyright notice in the Description page of Project Settings.


#include "BotGame.h"


void ABotGame::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!IsMovable && GetCharacterMovement()->Velocity.SizeSquared() < 5.f)
	{
		IsMovable = true;
		GetCharacterMovement()->SetMovementMode(MOVE_NavWalking);
	}
}

ABotGame::ABotGame()
{
	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollsionSphere"));
	CollisionSphere->InitSphereRadius(10.0f);

	bUseControllerRotationYaw = true;
	IsMovable = true;
}
void ABotGame::NotifyActorBeginOverlap(AActor * OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	if (OtherActor->IsA(ABotGame::StaticClass()))
	{
		UGameplayStatics::ApplyDamage(OtherActor, 10.0f, NULL, this, UDamageType::StaticClass());
	}
}

void ABotGame::FaceRotation(FRotator NewControlRotation, float DeltaTime)
{
	FRotator CurrentRotation = FMath::RInterpTo(GetActorRotation(), NewControlRotation, DeltaTime, 8.0f);
	Super::FaceRotation(CurrentRotation, DeltaTime);
}
