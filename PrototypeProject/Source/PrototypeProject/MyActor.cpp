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
	// ����Ÿ�� 1�� �޾ƿ��� ī��Ʈ �߰�
	if (DeltaTime >= 1)
	{
		DestoryCount++;
		DeltaTime = 0;
	}
	// ���� AI�� ���� ��� �̻��̸� ���� ������ �̵�.
	if (DestoryCount >= 2)
	{
	}
	Super::Tick(DeltaTime);

}

