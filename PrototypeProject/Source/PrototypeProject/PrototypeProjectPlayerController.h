// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "EngineMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PrototypeProjectPlayerController.generated.h"

UCLASS()
class APrototypeProjectPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	APrototypeProjectPlayerController();

protected:

	// Begin PlayerController interface
	virtual void PlayerTick(float DeltaTime) override;
	virtual void SetupInputComponent() override;
	// End PlayerController interface
};


