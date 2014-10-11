// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "Ballerts.h"
#include "BallertsGameMode.h"
#include "BallertsPlayerController.h"
#include "BallertsCharacter.h"
#include "GameFramework/HUD.h"

ABallertsGameMode::ABallertsGameMode(const class FPostConstructInitializeProperties& PCIP) : Super(PCIP)
{
	// use our custom PlayerController class
	PlayerControllerClass = ABallertsPlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Blueprints/Player"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
	static ConstructorHelpers::FClassFinder<AHUD> CustomHudClass(TEXT("/Game/Blueprints/MyHUD"));
	if (CustomHudClass.Class != NULL)
	{
		HUDClass = CustomHudClass.Class;
	}
}