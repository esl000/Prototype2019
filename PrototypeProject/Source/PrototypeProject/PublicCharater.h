// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "GameFramework/Character.h"
#include "PublicCharater.generated.h"

UENUM(Blueprintable)
enum class EAnimationState : uint8
{
	E_ATTACK,
	E_DASH,
	E_CHARGING,
	E_SKILL,
	E_MOVE,
	E_IDLE,
	E_HIT
};

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
	float PushingPower;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float Damage;

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = State)
	EAnimationState CurrentState;

	UPROPERTY(VisibleAnywhere, Category = "Collision")
	class UCapsuleComponent* CollisionCapsule;

	UPROPERTY(VisibleAnywhere, Category = "Weapon")
	class UStaticMeshComponent* WeaponMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Particle)
	TSubclassOf<AActor> PaticleActor;

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

	UFUNCTION()
	virtual void OnHitCollision(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable)
	virtual void EnableSwordOverlap(bool isEnable);
	UFUNCTION(BlueprintCallable)
	virtual void EndMotion();

	UFUNCTION(BlueprintCallable)
	virtual void PlayEffect(FVector loc, FRotator rot = FRotator::ZeroRotator);
	UFUNCTION(BlueprintCallable)
	virtual void Damage(float damage);
	UFUNCTION(BlueprintCallable)
	virtual void Die();

	virtual void PlayEffect(APublicCharater* target);
};
