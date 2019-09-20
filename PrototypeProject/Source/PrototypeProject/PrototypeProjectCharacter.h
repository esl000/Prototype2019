// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PrototypeProjectCharacter.generated.h"

UENUM(Blueprintable)
enum class EAnimationState : uint8 
{
	E_ATTACK,
	E_CHARGING,
	E_MOVE,
	E_IDLE,
	E_HIT
};

UCLASS(Blueprintable)
class APrototypeProjectCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	APrototypeProjectCharacter();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stat)
	float PushingPower;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = View)
	FVector LookDirection;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = View)
	float FocusYaw;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = View)
	float SpineRotationYaw;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = State)
	EAnimationState CurrentState;

	FVector DestLookDirection;

	// Called every frame.
	virtual void Tick(float DeltaSeconds) override;

	/** Returns TopDownCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns CursorToWorld subobject **/
	FORCEINLINE class UDecalComponent* GetCursorToWorld() { return CursorToWorld; }

	UFUNCTION(BlueprintCallable)
	void Attack();
	UFUNCTION(BlueprintCallable)
	void Charge();

private:


	/** Top down camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* TopDownCameraComponent;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** A decal that projects to the cursor location. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UDecalComponent* CursorToWorld;
};

