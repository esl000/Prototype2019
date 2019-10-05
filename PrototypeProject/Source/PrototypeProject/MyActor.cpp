// Fill out your copyright notice in the Description page of Project Settings.


#include "MyActor.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AMyActor::AMyActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}








int AMyActor::GETDC()
{
	return DestoryCount;
}

// Called when the game starts or when spawned
void AMyActor::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AMyActor::Tick(float DeltaTime)
{
	// 델다타임 1을 받아오면 카운트 추가
	if (DeltaTime >= 1)
	{
		DestoryCount++;
		DeltaTime = 0;
	}
	// 잡은 AI의 수가 몇마리 이상이면 다음 레벨로 이동.
	if (DestoryCount >= 2)
	{
	}
	Super::Tick(DeltaTime);

}

