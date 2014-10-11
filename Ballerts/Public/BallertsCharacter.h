// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/Character.h"
#include "BallertsCharacter.generated.h"

UCLASS(Blueprintable)
class ABallertsCharacter : public ACharacter
{
	GENERATED_UCLASS_BODY()

	/** Selection effect*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Effects)
	TSubobjectPtr<class UStaticMeshComponent> SelectedEffect;


	virtual bool UpdateNavigationRelevancy() override;

};

