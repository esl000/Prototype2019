// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "PrototypeProjectPlayerController.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Runtime/Engine/Classes/Components/DecalComponent.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "PrototypeProjectCharacter.h"
#include "Engine/World.h"
#include "DummyAICharacter.h"
#include "PrototypeProject.h"

APrototypeProjectPlayerController::APrototypeProjectPlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Crosshairs;
}

void APrototypeProjectPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
}

void APrototypeProjectPlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();
}

