// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EditorUtilityWidget.h"
#include "InGameUI.generated.h"

/**
 * 
 */
UCLASS()
class PROTOTYPEPROJECT_API UInGameUI : public UEditorUtilityWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class APrototypeProjectCharacter* Player;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DashPersent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AttackPersent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ChargePersent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SkillPersent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HPPersent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float StackPersent;
	
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
};
