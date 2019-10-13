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

	virtual void Tick(float DeltaTime) override;

	ABotGame();

	virtual void FaceRotation(FRotator NewControlRotation, float DeltaTime = 0.f) override;
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere, Category = Behavior)
		class UBehaviorTree* BotBehavior;
	UFUNCTION(BlueprintCallable)
	void PlayMeleeAnim();
	virtual void Die() override;

	virtual void OnHitCollision(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

private:
	FTimerHandle TimerHandle_EnemyStopAttack;
	bool DuringAttack;
};
