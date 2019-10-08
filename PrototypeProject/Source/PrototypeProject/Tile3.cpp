// Fill out your copyright notice in the Description page of Project Settings.


#include "Tile3.h"
FTimerHandle TimerHandle2;
int c = 0;
// Sets default values
ATile3::ATile3()
{
	Shape = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Shape;

	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATile3::BeginPlay()
{
	c = 0;
	Super::BeginPlay();

}

// Called every frame
void ATile3::Tick(float DeltaTime)
{
	if (!GetWorldTimerManager().IsTimerActive(TimerHandle2))
		GetWorldTimerManager().SetTimer(TimerHandle2, this, &ATile3::RepeatingFunction, 100.0f, false);

	if (c == 1)
		Shape->SetSimulatePhysics(true);
	if (GetActorLocation().Z <= -2000.0f)
		Destroy(this);
	Super::Tick(DeltaTime);

}

void ATile3::RepeatingFunction()
{
	GetWorldTimerManager().ClearTimer(TimerHandle2);
	c = 1;
}