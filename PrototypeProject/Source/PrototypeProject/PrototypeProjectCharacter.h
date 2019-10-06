// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "PublicCharater.h"
#include "PrototypeProjectCharacter.generated.h"

UENUM(Blueprintable)
enum class EAnimationState : uint8 
{
	E_ATTACK,
	E_DASH,
	E_CHARGING,
	E_SKILL,
	E_MOVE,
	E_IDLE,
	E_HIT
};

UCLASS(Blueprintable)
class APrototypeProjectCharacter : public APublicCharater
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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
	float WarkSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
	float DashSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
	float DashAcceletor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
	float CurrentDashSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float AttackCoolTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float ChargeCoolTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float SkillCoolTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float DashCoolTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float CurrentAttackCoolTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float CurrentChargeCoolTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float CurrentSkillCoolTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float CurrentDashCoolTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	bool bUnlockSkill;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
	TSubclassOf<class UUserWidget> InGameUIClass;

	FVector DashDirection;

	UPROPERTY()
	class UInGameUI* InGameUIInstance;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Effects)
	TSubclassOf<AActor> Particle;

	FVector DestLookDirection;

	int CurrentAttackCount;
	UPROPERTY(BlueprintReadWrite, Category = Stat)
	bool IgnoreAttackAnim;

	virtual void BeginPlay() override;

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
	UFUNCTION(BlueprintCallable)
	void UltimateSkill();
	UFUNCTION(BlueprintCallable)
	void Dash();
	UFUNCTION(BlueprintCallable)
	void EndDash();
	UFUNCTION(BlueprintCallable)
	void RotateSight();
	
	UFUNCTION(BlueprintImplementableEvent)
	void ApplyCameraShake(float value);


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

