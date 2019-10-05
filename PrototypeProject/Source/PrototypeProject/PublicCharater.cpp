// Fill out your copyright notice in the Description page of Project Settings.


#include "PublicCharater.h"

// Sets default values
APublicCharater::APublicCharater()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Stat.MaxHp = 100.f;
	Stat.Hp = Stat.MaxHp;
	Stat.IsMovable = true;
	Stat.MaxStack = 10;
	Stat.Stack = 0;
}

// Called when the game starts or when spawned
void APublicCharater::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void APublicCharater::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}



void APublicCharater::PlusDestory()
{
	DestoryCount++;
}

// Called to bind functionality to input
void APublicCharater::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

