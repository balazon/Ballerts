

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

	
	// Weight should contain the matrix elements of the weight function
	UFUNCTION(BlueprintCallable, Category = BalaUtilityFunctions)
	TArray<int32> MaxWeightMatching(TArray<float> Weights, const int32 N);

	UFUNCTION(BlueprintCallable, Category = BalaTestFunction)
	void JavitoUtas(TArray<float> Weights, const int32 N);
};
