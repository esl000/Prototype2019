// Fill out your copyright notice in the Description page of Project Settings.


#include "StartGame.h"
FTimerHandle StartT;
int d;
// Sets default values
AStartGame::AStartGame()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AStartGame::BeginPlay()
{
	d = 0;
	Super::BeginPlay();
	
}

// Called every frame
void AStartGame::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AStartGame::DestoryT()
{
	d = 1;
}

