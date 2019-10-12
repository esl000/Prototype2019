// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "GameFramework/Actor.h"
#include "LevelManager.generated.h"

UCLASS()
class PROTOTYPEPROJECT_API ALevelManager : public AActor
{
	GENERATED_BODY()
	
public:	

	static ALevelManager* Instance;

	// Sets default values for this actor's properties
	ALevelManager();

	UPROPERTY(VisibleAnywhere, Category = Pawn)
	TWeakObjectPtr<class APawn> UIPawn;

	UPROPERTY(VisibleAnywhere, Category = Pawn)
	TWeakObjectPtr<class APrototypeProjectCharacter> PlayerPawn;

	UPROPERTY(VisibleAnywhere, Category = Pawn)
	TWeakObjectPtr<class APawn> GameUIPawnInstance;

	UPROPERTY(VisibleAnywhere, Category = pawn)
	TWeakObjectPtr<class UGameUI> GameUIInstance;

	UPROPERTY(EditAnywhere, Category = GameSetting)
	TSubclassOf<class APrototypeProjectCharacter> CharacterPawn;
	
	UPROPERTY(EditAnywhere, Category = GameSetting)
	TSubclassOf<class ABotGame> EnemyPawn;

	UPROPERTY(EditAnywhere, Category = GameSetting)
	TSubclassOf<class UGameUI> GameUI;

	UPROPERTY(EditAnywhere, Category = GameSetting)
	TSubclassOf<class APawn> GameUIPawn;

	UPROPERTY(EditAnywhere, Category = GameSetting)
	TArray<FName> GameStageList;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stage)
	TArray<int32> StageTotalEnemy;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Stage)
	TArray<ABotGame*> StageEnemy;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Stage)
	TArray<AActor*> StageSpawnPoint;

	UPROPERTY(VisibleAnywhere, Category = Stage)
	int32 StageNotSpawnedEnemyCount;

	UPROPERTY(VisibleAnywhere, Category = Pawn)
	int32 CurrentStage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Spawn)
	FVector SpawnPoint;

	bool bIgnoreStageEndEvent;

	UFUNCTION(BlueprintCallable)
	static ALevelManager* GetInstance();

	UFUNCTION(BlueprintCallable)
	void StartGame();
	UFUNCTION(BlueprintCallable)
	void SetUpPlayer();
	UFUNCTION(BlueprintCallable)
	void SettingStage();
	UFUNCTION(BlueprintCallable)
	void PrevFinishStage();
	UFUNCTION(BlueprintCallable)
	void FinishStage();
	UFUNCTION(BlueprintCallable)
	void PrevRetryGame();
	UFUNCTION(BlueprintCallable)
	void StartRetryGame();
	UFUNCTION(BlueprintCallable)
	void RetryGame();
	UFUNCTION(BlueprintCallable)
	void GoToTitle();
	UFUNCTION(BlueprintCallable)
	void OnUnloadPrevStage();

	UFUNCTION(BlueprintCallable)
	void OnDeathEnemy(AActor* enemy);

protected:
	virtual void PostInitializeComponents() override;
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	APawn* GetGameUIPawn();

	FTimerHandle TimerHandle;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
