

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

	UFUNCTION(BlueprintImplementableEvent, Category = Commands)
	virtual void SetTargetLocation(const FVector& Location);

	UFUNCTION(BlueprintImplementableEvent, Category = Commands)
	virtual void SetTargetActor(AActor* Actor);
	
};
