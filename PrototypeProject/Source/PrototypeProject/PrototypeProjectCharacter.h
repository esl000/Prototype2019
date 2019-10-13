// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "PublicCharater.h"
#include "PrototypeProjectCharacter.generated.h"

UCLASS(Blueprintable)
class APrototypeProjectCharacter : public APublicCharater
{
	GENERATED_BODY()

public:
	APrototypeProjectCharacter();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = View)
	FVector LookDirection;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = View)
	float FocusYaw;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = View)
	float SpineRotationYaw;
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
	float AccelatorSkillPushingPower;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	bool bUnlockSkill;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	bool bUnlockDash;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
	TSubclassOf<class UUserWidget> InGameUIClass;

	FVector DashDirection;

	UPROPERTY()
	class UInGameUI* InGameUIInstance;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Projectle)
	TSubclassOf<AActor> SkillProjectle;

	FVector DestLookDirection;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attack)
	int CurrentAttackCount;

	virtual void BeginPlay() override;

	// Called every frame.
	virtual void Tick(float DeltaSeconds) override;

	/** Returns TopDownCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns CursorToWorld subobject **/
	FORCEINLINE class UDecalComponent* GetCursorToWorld() { return CursorToWorld; }


	TWeakObjectPtr<class UInputComponent> InputComponent;
	void ToggleInputComponent(bool isEnable);

	UFUNCTION(BlueprintCallable)
	void CheckEndAttack(int count);
	UFUNCTION(BlueprintCallable)
	void UltimateSkill();
	UFUNCTION(BlueprintCallable)
	void Dash();
	UFUNCTION(BlueprintCallable)
	void EndDash();
	UFUNCTION(BlueprintCallable)
	void RotateSight();

	UFUNCTION(BlueprintCallable)
	void SetPush(APublicCharater* other, float accelPersent);
	
	UFUNCTION(BlueprintImplementableEvent)
	void ApplyCameraShake(float value);

	virtual void OnHitCollision(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void PossessedBy(AController* newController) override;
	virtual void UnPossessed() override;
	virtual void Die() override;

private:

	void StartDash();
	void MoveForward(float delta);
	void MoveRight(float delta);
	void BasicAttack();
	void ChargingAttack();
	void UseSkill();
	void Hit();

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

