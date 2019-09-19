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

	// keep updating the destination every tick while desired
	if (bMoveToMouseCursor)
	{
		//MoveToMouseCursor();
	}
}

void APrototypeProjectPlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

	InputComponent->BindAction("SetDestination", IE_Pressed, this, &APrototypeProjectPlayerController::OnSetDestinationPressed);
	InputComponent->BindAction("Charging", IE_Pressed, this, &APrototypeProjectPlayerController::ChargingAttack);
	InputComponent->BindAction("SetDestination", IE_Released, this, &APrototypeProjectPlayerController::OnSetDestinationReleased);

	InputComponent->BindAxis("MoveForward", this, &APrototypeProjectPlayerController::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &APrototypeProjectPlayerController::MoveRight);

	// support touch devices 
	InputComponent->BindTouch(EInputEvent::IE_Pressed, this, &APrototypeProjectPlayerController::MoveToTouchLocation);
	InputComponent->BindTouch(EInputEvent::IE_Repeat, this, &APrototypeProjectPlayerController::MoveToTouchLocation);

	InputComponent->BindAction("ResetVR", IE_Pressed, this, &APrototypeProjectPlayerController::OnResetVR);
}

void APrototypeProjectPlayerController::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void APrototypeProjectPlayerController::MoveToMouseCursor()
{
	if (UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayEnabled())
	{
		if (APrototypeProjectCharacter* MyPawn = Cast<APrototypeProjectCharacter>(GetPawn()))
		{
			if (MyPawn->GetCursorToWorld())
			{
				UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, MyPawn->GetCursorToWorld()->GetComponentLocation());
			}
		}
	}
	else
	{
		// Trace to see what is under the mouse cursor
		FHitResult Hit;
		GetHitResultUnderCursor(ECC_Visibility, false, Hit);

		if (Hit.bBlockingHit)
		{
			// We hit something, move there
			SetNewMoveDestination(Hit.ImpactPoint);
		}
	}
}

void APrototypeProjectPlayerController::MoveToTouchLocation(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	FVector2D ScreenSpaceLocation(Location);

	// Trace to see what is under the touch location
	FHitResult HitResult;
	GetHitResultAtScreenPosition(ScreenSpaceLocation, CurrentClickTraceChannel, true, HitResult);
	if (HitResult.bBlockingHit)
	{
		// We hit something, move there
		SetNewMoveDestination(HitResult.ImpactPoint);
	}
}

void APrototypeProjectPlayerController::SetNewMoveDestination(const FVector DestLocation)
{
	APawn* const MyPawn = GetPawn();
	if (MyPawn)
	{
		float const Distance = FVector::Dist(DestLocation, MyPawn->GetActorLocation());

		// We need to issue move command only if far enough in order for walk animation to play correctly
		if ((Distance > 120.0f))
		{
			UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, DestLocation);
		}
	}
}

void APrototypeProjectPlayerController::ChargingAttack()
{
	UE_LOG(LogPrototypeProject, Warning, TEXT("press charging"));
	FHitResult result;
	FCollisionQueryParams params;
	FCollisionShape shape;
	shape.ShapeType = ECollisionShape::Type::Box;
	shape.Box.HalfExtentX = 50.f;
	shape.Box.HalfExtentY = 50.f;
	shape.Box.HalfExtentZ = 50.f;
	params.AddIgnoredActor(GetPawn());

	APrototypeProjectCharacter* MyPawn = Cast<APrototypeProjectCharacter>(GetPawn());


	if (GetWorld()->SweepSingleByChannel(result, GetPawn()->GetActorLocation(), GetPawn()->GetActorLocation() + 200.f * GetPawn()->GetActorForwardVector(), FQuat::Identity,
		ECollisionChannel::ECC_Pawn, shape, params))
	{
		ADummyAICharacter* character = Cast<ADummyAICharacter>(result.GetActor());
		if (character == nullptr)
			return;

		UCharacterMovementComponent* movement = Cast<UCharacterMovementComponent>(character->GetComponentByClass(UCharacterMovementComponent::StaticClass()));
		if (movement == nullptr)
			return;

		UE_LOG(LogPrototypeProject, Warning, TEXT("show charging"));

		movement->Velocity += (character->GetActorLocation() - GetPawn()->GetActorLocation()) * MyPawn->PushingPower * character->HitCount;
		character->HitCount = 0;
	}
}

void APrototypeProjectPlayerController::OnSetDestinationPressed()
{
	// set flag to keep updating destination until released
	bMoveToMouseCursor = true;
	
	FHitResult result;
	FCollisionQueryParams params;
	FCollisionShape shape;
	shape.ShapeType = ECollisionShape::Type::Box;
	shape.Box.HalfExtentX = 50.f;
	shape.Box.HalfExtentY = 50.f;
	shape.Box.HalfExtentZ = 50.f;
	params.AddIgnoredActor(GetPawn());

	APrototypeProjectCharacter* MyPawn = Cast<APrototypeProjectCharacter>(GetPawn());


	if (GetWorld()->SweepSingleByChannel(result, GetPawn()->GetActorLocation(), GetPawn()->GetActorLocation() + 200.f * GetPawn()->GetActorForwardVector(), FQuat::Identity,
		ECollisionChannel::ECC_Pawn, shape, params))
	{
		ADummyAICharacter* character = Cast<ADummyAICharacter>(result.GetActor());
		if (character == nullptr)
			return;

		UE_LOG(LogPrototypeProject, Warning, TEXT("add hitcount"));

		if(character->HitCount <= 10)
			character->HitCount++;
	}

}

void APrototypeProjectPlayerController::OnSetDestinationReleased()
{
	// clear flag to indicate we should stop updating the destination
	bMoveToMouseCursor = false;
}

void APrototypeProjectPlayerController::MoveForward(float delta)
{
	if (APrototypeProjectCharacter* MyPawn = Cast<APrototypeProjectCharacter>(GetPawn()))
	{
		MyPawn->GetCharacterMovement()->AddInputVector(FVector(delta, 0.f, 0.f));
	}
}

void APrototypeProjectPlayerController::MoveRight(float delta)
{
	if (APrototypeProjectCharacter* MyPawn = Cast<APrototypeProjectCharacter>(GetPawn()))
	{
		MyPawn->GetCharacterMovement()->AddInputVector(FVector(0.f, delta, 0.f));
	}
}
