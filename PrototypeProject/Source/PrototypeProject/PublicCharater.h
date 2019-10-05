// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PublicCharater.generated.h"

USTRUCT(Atomic, BlueprintType)
struct PROTOTYPEPROJECT_API FCharacterStat
{
	GENERATED_USTRUCT_BODY()
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float MaxHp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float Hp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	int32 MaxStack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	int32 Stack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	bool IsMovable;
};

UCLASS()
class PROTOTYPEPROJECT_API APublicCharater : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APublicCharater();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	FCharacterStat Stat;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = State)
		int DestoryCount = 0;

	UFUNCTION(BlueprintCallable, Category = Check)
		void PlusDestory();
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
