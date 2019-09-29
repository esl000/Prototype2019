// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "GameFramework/Actor.h"
#include "MyActor.generated.h"

UCLASS()
class PROTOTYPEPROJECT_API AMyActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMyActor();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = State1)
		int DestoryCount = 0;

	UFUNCTION(BlueprintCallable, Category = Check1)
		void PlusDestory();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
