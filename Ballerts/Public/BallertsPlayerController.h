// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
#pragma once

#include "GameFramework/PlayerController.h"

#include "BallertsCharacter.h"
#include "BallertsPlayerController.generated.h"


UCLASS()
class ABallertsPlayerController : public APlayerController
{
	GENERATED_UCLASS_BODY()

	UFUNCTION(BlueprintCallable, Category = Commands)
	void DeselectAll();

	UFUNCTION(BlueprintCallable, Category = Commands)
	void MoveToFormation();

	UFUNCTION(BlueprintCallable, Category = Commands)
	TArray<FVector> GetTargets();
protected:

	TArray<FVector> CurrentTargets;
	/** True if the controlled character should navigate to the mouse cursor. */
	uint32 bMoveToMouseCursor : 1;

	TArray<ABallertsCharacter*> SelectedUnits;

	void AddUnitToSelection(ABallertsCharacter* Unit);

	void RemoveUnitFromSelection(ABallertsCharacter* Unit);



	float Delta;



	// Begin PlayerController interface
	virtual void PlayerTick(float DeltaTime) override;
	virtual void SetupInputComponent() override;
	virtual void BeginPlay() override;
	// End PlayerController interface

	/** Navigate player to the current mouse cursor location. */
	void MoveToMouseCursor();

	/** Follow Actor*/
	void MoveToActor(AActor* TargetActor);

	/** Navigate player to the current touch location. */
	void MoveToTouchLocation(const ETouchIndex::Type FingerIndex, const FVector Location);
	
	/** Navigate player to the given world location. */
	void SetNewMoveDestination(const FVector DestLocation);

	/** Input handlers for SetDestination action. */
	void OnSetDestinationPressed();
	void OnSetDestinationReleased();

	void OnClickPressed();
	void OnClickReleased();

	void OnIncreaseSelectionPressed();
	void OnIncreaseSelectionReleased();

	

	void OnZoomStart();

	void OnZoomProcessing();
	
	void OnPanVerticalStart(float value);

	void OnPanVerticalProcessing(float value);


	void OnPanHorizontalStart(float value);

	void OnPanHorizontalProcessing(float value);

	void OnRightClickPressed();

	void OnTestButtonPressed();


	TArray<FVector2D> TraversalPointsOnPath(TArray<FVector2D> path, int pointCount, bool closed);
};


