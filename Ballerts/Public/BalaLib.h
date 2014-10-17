// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "BalaLib.generated.h"

/**
 * 
 */
UCLASS()
class BALLERTS_API UBalaLib : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()

	
	UFUNCTION(BlueprintCallable, Category = BalaTestFunction)
	static void Assignment(TArray<float> Weights, const int32 N, TArray<int32>& Result);
};
