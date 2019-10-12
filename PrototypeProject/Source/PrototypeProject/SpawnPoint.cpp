// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnPoint.h"
#include "LevelManager.h"

// Sets default values
ASpawnPoint::ASpawnPoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bIsSpawnAble = true;
}

// Called when the game starts or when spawned
void ASpawnPoint::BeginPlay()
{
	Super::BeginPlay();
	ALevelManager::GetInstance()->StageSpawnPoint.Add(this);
}

// Called every frame
void ASpawnPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bIsSpawnAble)
		return;

	FHitResult result;
	if (!GetWorld()->LineTraceSingleByChannel(result, GetActorLocation(),
		GetActorLocation() - FVector(0.f, 0.f, 300.f), ECC_Visibility))
	{
		bIsSpawnAble = false;
		ALevelManager::GetInstance()->StageSpawnPoint.Remove(this);
	}
}

