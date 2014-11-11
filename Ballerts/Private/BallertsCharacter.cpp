// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "Ballerts.h"
#include "BallertsCharacter.h"

ABallertsCharacter::ABallertsCharacter(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	// Set size for player capsule
	CapsuleComponent->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	CharacterMovement->bOrientRotationToMovement = true; // Rotate character to moving direction
	CharacterMovement->RotationRate = FRotator(0.f, 640.f, 0.f);
	CharacterMovement->bConstrainToPlane = true;
	CharacterMovement->bSnapToPlaneAtStart = true;

	

	//Creating selection effect mesh
	SelectedEffect = PCIP.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("SelectedEffect"));
	SelectedEffect->AttachTo(RootComponent);
	SelectedEffect->SetVisibility(false);

	UnitGroup = NULL;
}


void ABallertsCharacter::UpdateNavigationRelevance()
{
	bCanAffectNavigationGeneration = true;
	//CapsuleComponent->bNavigationRelevant = true;
	Super::UpdateNavigationRelevance();
	
	//SetCanAffectNavigationGeneration(true);
	
	//CapsuleComponent->SetCanEverAffectNavigation(true);
}

//UUnitGroup* ABallertsCharacter::GetUnitGroup()
//{
//	return UnitGroup;
//}


FVector2D ABallertsCharacter::GetActorLocation2D()
{
	FVector Loc = GetActorLocation();
	return FVector2D(Loc.X, Loc.Y);
}