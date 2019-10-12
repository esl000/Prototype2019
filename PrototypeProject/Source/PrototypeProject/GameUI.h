// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameUI.generated.h"

/**
 * 
 */
UCLASS()
class PROTOTYPEPROJECT_API UGameUI : public UUserWidget
{
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintImplementableEvent)
	void StartClearUI();
	UFUNCTION(BlueprintImplementableEvent)
	void EndClearUI();
	UFUNCTION(BlueprintImplementableEvent)
	void StartRetryUI();
	UFUNCTION(BlueprintImplementableEvent)
	void EndRetryUI();
};
