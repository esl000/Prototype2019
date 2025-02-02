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
	/** True if the controlled character should navigate to the mouse cursor. */
	uint32 bMoveToMouseCursor : 1;

	// Begin PlayerController interface
	virtual void PlayerTick(float DeltaTime) override;
	virtual void SetupInputComponent() override;
	// End PlayerController interface

	/** Resets HMD orientation in VR. */
	void OnResetVR();

	/** Navigate player to the current mouse cursor location. */
	void MoveToMouseCursor();

	/** Navigate player to the current touch location. */
	void MoveToTouchLocation(const ETouchIndex::Type FingerIndex, const FVector Location);
	
	/** Navigate player to the given world location. */
	void SetNewMoveDestination(const FVector DestLocation);

	void ChargingAttack();

	/** Input handlers for SetDestination action. */
	void BasicAttack();
	void OnSetDestinationReleased();

	void StartDash();

	void MoveForward(float delta);
	void MoveRight(float delta);
};


