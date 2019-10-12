// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "AIController.h"
#include "AAIController.generated.h"

/**
 *
 */
class UBehaviorTreeComponent;
class UBlackboardComponent;
UCLASS(config = Game)
class PROTOTYPEPROJECT_API AAAIController : public AAIController
{
	GENERATED_BODY()
public:
	AAAIController();


	UFUNCTION(BlueprintCallable)
	virtual void OnPossess(APawn* InPawn) override;


	void SetEnemy(class APawn* InPawn);

	virtual void UpdateControlRotation(float DeltaTime, bool bUpdatePawn = true);
	UFUNCTION(BlueprintCallable, Category = Behavior)
		void FindClosestEnemy();

	UFUNCTION(BlueprintCallable, Category = Behavior)
		void AttackEnemy();
	class APrototypeProjectCharacter* GetEnemy() const;

	void MoveAccess();
private:
	UPROPERTY(transient)
		UBlackboardComponent* BlackboardComp;
	UPROPERTY(transient)
		UBehaviorTreeComponent* BehaviorComp;
protected:
	int32 EnemyKeyID;
	int32 AttackCan;
	float dist1;
	int32 Cdist;
	FTimerHandle TimeHandler;
	int32 e;
};
