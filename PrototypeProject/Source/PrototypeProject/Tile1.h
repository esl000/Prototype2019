// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "GameFramework/Actor.h"
#include "Tile1.generated.h"

UCLASS()
class PROTOTYPEPROJECT_API ATile1 : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATile1();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void RepeatingFunction();

private:
	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* Shape;
};
