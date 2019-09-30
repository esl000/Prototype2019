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
UCLASS()
class PROTOTYPEPROJECT_API ABotGame : public APublicCharater
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stat)
		int HitCount;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stat)
		bool IsMovable;

	virtual void Tick(float DeltaTime) override;

	ABotGame();
	UPROPERTY(VisibleAnywhere, Category = "Collision")
		class USphereComponent* CollisionSphere;

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	virtual void FaceRotation(FRotator NewControlRotation, float DeltaTime = 0.f) override;
	UPROPERTY(EditAnywhere, Category = Behavior)
		class UBehaviorTree* BotBehavior;
	void PlayMeleeAnim();
	void StopMeleeAnim();
	virtual void Tick(float DeltaTime) override;
protected:
	UPROPERTY(EditDefaultsOnly, Category = Pawn)
		UAnimMontage* MeleeAnim;

private:
	FTimerHandle TimerHandle_EnemyStopAttack;

	bool DuringAttack;
};
