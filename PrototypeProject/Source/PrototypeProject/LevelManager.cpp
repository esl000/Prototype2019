// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelManager.h"
#include "PrototypeProjectCharacter.h"
#include "Engine/LevelStreaming.h"
#include "BotGame.h"
#include "PrototypeProject.h"
#include "GameUI.h"
#include "Blueprint/UserWidget.h"

ALevelManager* ALevelManager::Instance = nullptr;

// Sets default values
ALevelManager::ALevelManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bIgnoreStageEndEvent = true;
}

ALevelManager* ALevelManager::GetInstance()
{
	return Instance;
}

void ALevelManager::StartGame()
{
	//static ConstructorHelpers::FClassFinder<APrototypeProjectCharacter> Character(TEXT("Blueprint'/Game/TopDownCPP/Blueprints/TopDownCharacter.TopDownCharacter_C'"));

	//if (!Character.Succeeded())
	//	return;

	FLatentActionInfo ActionInfo = FLatentActionInfo();
	ActionInfo.CallbackTarget = this;
	ActionInfo.ExecutionFunction = FName("SetUpPlayer");
	ActionInfo.UUID = 2019;
	ActionInfo.Linkage = 0;

	UGameplayStatics::GetStreamingLevel(GetWorld(), GameStageList[0])->
		OnLevelLoaded.AddUniqueDynamic(this, &ALevelManager::SetUpPlayer);
	UGameplayStatics::LoadStreamLevel(GetWorld(), GameStageList[0], true, true, FLatentActionInfo());
	CurrentStage = 0;
}

void ALevelManager::SetUpPlayer()
{
	UGameplayStatics::GetStreamingLevel(GetWorld(), GameStageList[CurrentStage])->
		OnLevelLoaded.RemoveDynamic(this, &ALevelManager::SetUpPlayer);

	if (GameUIInstance.IsValid() && GameUIInstance->IsInViewport())
		GameUIInstance->RemoveFromViewport();

	if (!PlayerPawn.IsValid())
		PlayerPawn = GetWorld()->SpawnActor<APrototypeProjectCharacter>(CharacterPawn, SpawnPoint, FRotator::ZeroRotator);
	else
	{
		PlayerPawn->SetActorLocation(SpawnPoint);
		PlayerPawn->SetActorRotation(FRotator::ZeroRotator);
	}

	GetWorld()->GetFirstPlayerController()->Possess(PlayerPawn.Get());
	bIgnoreStageEndEvent = false;

	FTimerDelegate timerDelegate;

	timerDelegate.BindLambda([this]() { SettingStage(); });
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, timerDelegate, 3.f, false);
}

void ALevelManager::SettingStage()
{
	for (int i = 0; i < StageSpawnPoint.Num(); ++i)
	{
		int index = StageEnemy.Add(GetWorld()->SpawnActor<ABotGame>(EnemyPawn,
			StageSpawnPoint[i]->GetActorLocation(), StageSpawnPoint[i]->GetActorRotation()));
		StageEnemy[index]->OnDestroyed.AddUniqueDynamic(this, &ALevelManager::OnDeathEnemy);
	}
	StageNotSpawnedEnemyCount = StageTotalEnemy[CurrentStage];
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
}

void ALevelManager::PrevFinishStage()
{
	if (bIgnoreStageEndEvent)
		return;
	bIgnoreStageEndEvent = true;

	StageSpawnPoint.Empty();

	if (GameStageList.Num() - 1 == CurrentStage)
	{
		GoToTitle();
		return;
	}
	else
	{
		PlayerPawn->GetCharacterMovement()->GravityScale = 0.f;
		GetWorld()->GetFirstPlayerController()->Possess(GetGameUIPawn());

		if (!GameUIInstance.IsValid())
		{
			GameUIInstance = CreateWidget<UGameUI>(GetWorld()->GetFirstPlayerController(), GameUI);
			GameUIInstance->AddToViewport();
			GameUIInstance->StartClearUI();
		}
		else
		{
			GameUIInstance->AddToViewport();
			GameUIInstance->StartClearUI();
		}
	}
}

void ALevelManager::FinishStage()
{
	UGameplayStatics::GetStreamingLevel(GetWorld(), GameStageList[CurrentStage])->
		OnLevelUnloaded.AddUniqueDynamic(this, &ALevelManager::OnUnloadPrevStage);
	UGameplayStatics::UnloadStreamLevel(GetWorld(), GameStageList[CurrentStage], FLatentActionInfo(), false);
}

void ALevelManager::PrevRetryGame()
{
	if (bIgnoreStageEndEvent)
		return;
	bIgnoreStageEndEvent = true;

	StageSpawnPoint.Empty();
	for (int i = 0; i < StageEnemy.Num(); ++i)
	{
		StageEnemy[i]->OnDestroyed.RemoveDynamic(this, &ALevelManager::OnDeathEnemy);
		StageEnemy[i]->Destroy();
	}
	StageEnemy.Empty();

	GetWorld()->GetFirstPlayerController()->Possess(GetGameUIPawn());

	if (!GameUIInstance.IsValid())
	{
		GameUIInstance = CreateWidget<UGameUI>(GetWorld()->GetFirstPlayerController(), GameUI);
		GameUIInstance->AddToViewport();
		GameUIInstance->StartRetryUI();
	}
	else
	{
		GameUIInstance->AddToViewport();
		GameUIInstance->StartRetryUI();
	}
}

void ALevelManager::StartRetryGame()
{

	UGameplayStatics::GetStreamingLevel(GetWorld(), GameStageList[CurrentStage])->
		OnLevelUnloaded.AddUniqueDynamic(this, &ALevelManager::RetryGame);
	UGameplayStatics::UnloadStreamLevel(GetWorld(), GameStageList[CurrentStage], FLatentActionInfo(), false);
}

void ALevelManager::RetryGame()
{
	UGameplayStatics::GetStreamingLevel(GetWorld(), GameStageList[CurrentStage])->
		OnLevelUnloaded.RemoveDynamic(this, &ALevelManager::RetryGame);
	StartGame();
}

void ALevelManager::GoToTitle()
{
	if (GameUIInstance.IsValid() && GameUIInstance->IsInViewport())
		GameUIInstance->RemoveFromViewport();

	StageSpawnPoint.Empty();

	for (int i = 0; i < StageEnemy.Num(); ++i)
	{
		StageEnemy[i]->OnDestroyed.RemoveDynamic(this, &ALevelManager::OnDeathEnemy);
		StageEnemy[i]->Destroy();
	}
	StageEnemy.Empty();

	UE_LOG(LogPrototypeProject, Warning, TEXT("Go to Title"));
	GetWorld()->GetFirstPlayerController()->Possess(UIPawn.Get());
	UGameplayStatics::UnloadStreamLevel(GetWorld(), GameStageList[CurrentStage], FLatentActionInfo(), false);
}

void ALevelManager::OnUnloadPrevStage()
{
	UGameplayStatics::GetStreamingLevel(GetWorld(), GameStageList[CurrentStage])->
		OnLevelUnloaded.RemoveDynamic(this, &ALevelManager::OnUnloadPrevStage);

	CurrentStage++;

	UGameplayStatics::GetStreamingLevel(GetWorld(), GameStageList[CurrentStage])->
		OnLevelLoaded.AddUniqueDynamic(this, &ALevelManager::SetUpPlayer);
	UGameplayStatics::LoadStreamLevel(GetWorld(), GameStageList[CurrentStage], true, true, FLatentActionInfo());
}

void ALevelManager::OnDeathEnemy(AActor * enemy)
{
	if (StageEnemy.FindByPredicate([enemy](ABotGame* element)
	{
		return element == enemy;
	}))
	{
		StageEnemy.Remove(Cast<ABotGame>(enemy));

		if (StageNotSpawnedEnemyCount > 0 && StageSpawnPoint.Num() != 0)
		{
			int spawnIndex = FMath::RandRange(0, StageSpawnPoint.Num() - 1);

			int index = StageEnemy.Add(GetWorld()->SpawnActor<ABotGame>(EnemyPawn,
				StageSpawnPoint[spawnIndex]->GetActorLocation(), StageSpawnPoint[spawnIndex]->GetActorRotation()));
			StageEnemy[index]->OnDestroyed.AddUniqueDynamic(this, &ALevelManager::OnDeathEnemy);
			StageNotSpawnedEnemyCount--;
		}

		if (StageEnemy.Num() == 0)
			PrevFinishStage();
	}
}

//void ALevelManager::OnLoadComplete()
//{
//	//const TSet<ULevel*>* levels = &GetWorld()->GetActiveLevelCollection()->GetLevels();
//	//for (auto iter = levels->CreateConstIterator(); iter; ++iter)
//	//{
//	//	ULevel* element = Cast<ULevel>(*iter);
//	//	if (element->GetFName().IsEqual(TEXT("GameStage1")))
//	//	{
//	//		GetWorld()->SpawnActor(T)
//	//	}
//	//}
//}

void ALevelManager::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	Instance = this;
}

// Called when the game starts or when spawned
void ALevelManager::BeginPlay()
{
	Super::BeginPlay();
	UIPawn = GetWorld()->GetFirstPlayerController()->GetPawn();
}

APawn * ALevelManager::GetGameUIPawn()
{
	if (!GameUIPawnInstance.IsValid())
		GameUIPawnInstance = GetWorld()->SpawnActor<APawn>(GameUIPawn, SpawnPoint, FRotator::ZeroRotator);
	return GameUIPawnInstance.Get();
}

// Called every frame
void ALevelManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

