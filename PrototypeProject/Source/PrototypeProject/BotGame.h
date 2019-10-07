// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "PublicCharater.h"
#include "MyActor.h"
#include "EngineUtils.h"
#include "BotGame.generated.h"

/**
 *
 */
UENUM(Blueprintable)
enum class EAnimationState1 : uint8
{
	E_ATTACK,
	E_DASH,
	E_CHARGING,
	E_SKILL,
	E_MOVE,
	E_IDLE,
	E_HIT
};

UCLASS()
class PROTOTYPEPROJECT_API ABotGame : public APublicCharater
{
	GENERATED_BODY()

public:

	virtual void Tick(float DeltaTime) override;

	ABotGame();
	UPROPERTY(VisibleAnywhere, Category = "Collision")
		class UCapsuleComponent* CollisionCapsule;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = State)
	EAnimationState1 CurrentState;
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	virtual void FaceRotation(FRotator NewControlRotation, float DeltaTime = 0.f) override;
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere, Category = Behavior)
		class UBehaviorTree* BotBehavior;
	UFUNCTION(BlueprintCallable)
	void PlayMeleeAnim();
	UFUNCTION(BlueprintCallable)
	void StopMeleeAnim();

	UFUNCTION()
	void OnHitCollision(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable)
	void Attack();
	UFUNCTION(BlueprintCallable)
	void Charge();
	UFUNCTION(BlueprintCallable)
	void EndAttack();
	UFUNCTION(BlueprintCallable)
	void EndCharge();

protected:
	UPROPERTY(EditDefaultsOnly, Category = Pawn)
		UAnimMontage* MeleeAnim;

private:
	FTimerHandle TimerHandle_EnemyStopAttack;
	bool DuringAttack;
};
