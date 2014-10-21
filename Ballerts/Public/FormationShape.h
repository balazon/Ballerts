// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "FormationShape.generated.h"

/**
 * 
 */
UCLASS()
class BALLERTS_API UFormationShape : public UObject
{
	GENERATED_UCLASS_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Formation)
	TArray<FVector2D> Points;


	UFUNCTION(BlueprintNativeEvent, Category = Formation)
	void SetATriangle(const FVector2D& Center, float side, const int32 N);
};
