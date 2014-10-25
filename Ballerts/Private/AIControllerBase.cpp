

#include "Ballerts.h"
#include "AIControllerBase.h"


AAIControllerBase::AAIControllerBase(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{

}


void AAIControllerBase::SetTargetActor_Implementation(AActor* Actor)
{

}

void AAIControllerBase::SetTargetLocation_Implementation(const FVector& Location)
{

}

void AAIControllerBase::SetTargetLeader_Implementation(AActor* Actor, const FVector& RelVectorFromLeader)
{

}

void AAIControllerBase::SetTargetLocationAsLeader_Implementation(const FVector& Location, const TArray<ABallertsCharacter*>& Followers)
{
	
}