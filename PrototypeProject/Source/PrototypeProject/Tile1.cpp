// Fill out your copyright notice in the Description page of Project Settings.


#include "Tile1.h"
FTimerHandle TimerHandle;
int a = 0;
// Sets default values
ATile1::ATile1()
{
	Shape = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Shape;
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATile1::BeginPlay()
{
	a = 0;
	Super::BeginPlay();

}

// Called every frame
void ATile1::Tick(float DeltaTime)
{
	if (!GetWorldTimerManager().IsTimerActive(TimerHandle))
		GetWorldTimerManager().SetTimer(TimerHandle, this, &ATile1::RepeatingFunction, 30.0f, false);

	if (a == 1)
		Shape->SetSimulatePhysics(true);
	if (GetActorLocation().Z <= -2000.0f)
		Destroy(this);
	Super::Tick(DeltaTime);

}


void ATile1::RepeatingFunction()
{
	GetWorldTimerManager().ClearTimer(TimerHandle);
	a = 1;
}