// Fill out your copyright notice in the Description page of Project Settings.


#include "DummyAICharacter.h"

// Sets default values
ADummyAICharacter::ADummyAICharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	HitCount = 0;

}

// Called when the game starts or when spawned
void ADummyAICharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADummyAICharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ADummyAICharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

