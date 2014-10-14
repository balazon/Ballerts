

#pragma once

#include "AIController.h"
#include "AIControllerBase.generated.h"

/**
 * 
 */
UCLASS()
class BALLERTS_API AAIControllerBase : public AAIController
{
	GENERATED_UCLASS_BODY()

	UFUNCTION(BlueprintNativeEvent, Category = Commands)
	void SetTargetLocation(const FVector& Location);

	UFUNCTION(BlueprintNativeEvent, Category = Commands)
	void SetTargetActor(AActor* Actor);
	
};
