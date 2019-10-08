// Fill out your copyright notice in the Description page of Project Settings.


#include "Tile2.h"
FTimerHandle TimerHandle1;
int b = 0;
// Sets default values
ATile2::ATile2()
{
	Shape = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Shape;

	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATile2::BeginPlay()
{
	b = 0;
	Super::BeginPlay();

}

// Called every frame
void ATile2::Tick(float DeltaTime)
{
	if (!GetWorldTimerManager().IsTimerActive(TimerHandle1))
		GetWorldTimerManager().SetTimer(TimerHandle1, this, &ATile2::RepeatingFunction, 200.0f, false);

	if (b == 1)
		Shape->SetSimulatePhysics(true);
	if (GetActorLocation().Z <= -2000.0f)
		Destroy(this);
	Super::Tick(DeltaTime);

}

void ATile2::RepeatingFunction()
{
	GetWorldTimerManager().ClearTimer(TimerHandle1);
	b = 1;
}