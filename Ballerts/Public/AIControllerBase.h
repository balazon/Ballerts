

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

	UFUNCTION(BlueprintNativeEvent, Category = Commands)
	void SetTargetLeader(AActor* Actor, const FVector& RelVectorFromLeader);
	

	UFUNCTION(BlueprintNativeEvent, Category = Commands)
	void SetTargetLocationAsLeader(const FVector& Location, const TArray<AActor*>& Followers);
};
