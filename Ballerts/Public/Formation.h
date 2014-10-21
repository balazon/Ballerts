// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"


#include "FormationShape.h"
#include "Formation.generated.h"

/**
 * 
 */
UCLASS()
class BALLERTS_API UFormation : public UObject
{
	GENERATED_UCLASS_BODY()


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Formation)
	TArray<UFormationShape*> Shapes;

	UFUNCTION(BlueprintNativeEvent, Category = Formation)
	void SetATriangle(const FVector2D& Center, float side, const int32 N);

	UFUNCTION(BlueprintNativeEvent, Category = Formation)
	TArray<FVector2D> AllPoints();
};
