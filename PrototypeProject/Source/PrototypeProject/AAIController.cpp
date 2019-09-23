// Fill out your copyright notice in the Description page of Project Settings.


#include "AAIController.h"
#include "BotGame.h"
#include "PrototypeProjectCharacter.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"

AAAIController::AAAIController()
{
	BlackboardComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackBoardComp"));
	BrainComponent = BehaviorComp = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorComp"));
}
void AAAIController::OnPossess(APawn * InPawn)
{
	Super::OnPossess(InPawn);

	ABotGame* Bot = Cast<ABotGame>(InPawn);
	if (Bot && Bot->BotBehavior)
	{
		if (Bot->BotBehavior->BlackboardAsset)
		{
			BlackboardComp->InitializeBlackboard(*Bot->BotBehavior->BlackboardAsset);
		}
		EnemyKeyID = BlackboardComp->GetKeyID("Enemy");
		BehaviorComp->StartTree(*(Bot->BotBehavior));
	}
}

void AAAIController::SetEnemy(APawn * InPawn)
{
	if (BlackboardComp)
	{
		BlackboardComp->SetValue<UBlackboardKeyType_Object>(EnemyKeyID, InPawn);
		SetFocus(InPawn);
	}
}

void AAAIController::UpdateControlRotation(float DeltaTime, bool bUpdatePawn)
{
	FVector FocalPoint = GetFocalPoint();
	if (!FocalPoint.IsZero() && GetPawn())
	{
		FVector Direction = FocalPoint - GetPawn()->GetActorLocation();
		FRotator NewControlRotation = Direction.Rotation();
		NewControlRotation.Yaw = FRotator::ClampAxis(NewControlRotation.Yaw);

		SetControlRotation(NewControlRotation);

		APawn* const P = GetPawn();
		if (P && bUpdatePawn)
		{
			P->FaceRotation(NewControlRotation, DeltaTime);
		}
	}
}

void AAAIController::FindClosestEnemy()
{
	APawn* MyBot = GetPawn();
	if (MyBot == NULL)
	{
		return;
	}

	const FVector MyLocal = MyBot->GetActorLocation();
	float BestDistSq = 1500000.0f;
	APrototypeProjectCharacter* BestPawn = NULL;

	for (FConstPawnIterator It = GetWorld()->GetPawnIterator(); It; ++It)
	{
		APrototypeProjectCharacter* TestPawn = Cast<APrototypeProjectCharacter>(*It);
		if (TestPawn)
		{
			const float DistSq = (TestPawn->GetActorLocation() - MyLocal).SizeSquared();

			if (DistSq < BestDistSq)
			{
				BestDistSq = DistSq;
				BestPawn = TestPawn;
			}
		}
	}
	if (BestPawn)
	{
		SetEnemy(BestPawn);
	}
}