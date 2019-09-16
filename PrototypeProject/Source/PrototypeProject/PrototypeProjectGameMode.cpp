// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "PrototypeProjectGameMode.h"
#include "PrototypeProjectPlayerController.h"
#include "PrototypeProjectCharacter.h"
#include "UObject/ConstructorHelpers.h"

APrototypeProjectGameMode::APrototypeProjectGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = APrototypeProjectPlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDownCPP/Blueprints/TopDownCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}