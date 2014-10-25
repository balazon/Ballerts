// Fill out your copyright notice in the Description page of Project Settings.

#include "Ballerts.h"
#include "UnitGroup.h"

#include "AI/Navigation/NavigationSystem.h"
#include "AI/Navigation/NavigationPath.h"

#include "BallertsCharacter.h"
#include "AIControllerBase.h"

#include "BalaLib.h"
#include "Formation.h"


UUnitGroup::UUnitGroup(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	Formation = NewObject<class UFormation>();
	CurrentShape = EShapeEnum::SE_NONE;
}


void UUnitGroup::MoveToFormationTriangle_Implementation()
{
	//calculate the triangle's shape
	FVector2D Center = FVector2D(0.f, 0.f);
	int count = 0;
	float sumCapDiameter = 0.f;
	for (ABallertsCharacter* MyChar : Units)
	{
		if (MyChar)
		{
			FVector loc = MyChar->GetActorLocation();
			UCapsuleComponent* cap = Cast<UCapsuleComponent>(MyChar->GetComponentByClass(UCapsuleComponent::StaticClass()));
			sumCapDiameter += cap->GetScaledCapsuleRadius() * 2.f;
			Center += FVector2D(loc.X, loc.Y);
			count++;
		}
	}

	Center /= count;
	UE_LOG(LogTemp, Warning, TEXT("Center: %.2f %.2f"), Center.X, Center.Y);



	float triEdge = sumCapDiameter / 3.f * 4.f;

	Formation->SetATriangle(Center, triEdge, Units.Num());

	TArray<FVector2D> res = Formation->AllPoints();

	//pair the destinations with units
	TArray<int32> indexAssigns;
	TArray<float> Weights;
	Weights.Init(Units.Num() * Units.Num());
	for (int i = 0; i < Units.Num(); i++)
	{
		ABallertsCharacter* MyChar = Units[i];
		FVector Pos = MyChar->GetActorLocation();
		FVector2D Loc2D = FVector2D(Pos.X, Pos.Y);
		for (int j = 0; j < Units.Num(); j++)
		{
			Weights[i * Units.Num() + j] = -FVector2D::Distance(Loc2D, res[j]);
		}
	}
	UBalaLib::Assignment(Weights, Units.Num(), indexAssigns);

	//issue commands to move there
	for (int32 i = 0; i < Units.Num(); i++)
	{
		ABallertsCharacter* MyChar = Units[i];
		if (MyChar)
		{
			AAIControllerBase* Controller = Cast<AAIControllerBase>(MyChar->GetController());
			Controller->SetTargetLocation(FVector(res[indexAssigns[i]], 120.f));
			//UE_LOG(LogTemp, Warning, TEXT("Path: %.2f %.2f"), res[indexAssigns[i]].X, res[indexAssigns[i]].Y);
		}
	}
}



void UUnitGroup::MoveToFormation_Implementation(const TEnumAsByte<EShapeEnum::Type>& ShapeType)
{
	CurrentShape = ShapeType;
	switch (ShapeType)
	{
	case EShapeEnum::SE_TRIANGLE:
		MoveToFormationTriangle();
		break;
	case EShapeEnum::SE_SQUARE:
		break;
	case EShapeEnum::SE_NONE:
		break;
	}
}


void UUnitGroup::SetDestination_Implementation(const FVector& Destination)
{
	CurrentDestination = Destination;

	CurrentDestination.Z = 120.f;


	float minDistance = 10000.f;
	ABallertsCharacter* ClosestChar = NULL;
	UNavigationSystem* const NavSys = World->GetNavigationSystem();

	//UE_LOG(LogTemp, Warning, TEXT("%.3f"), DestLocation.Z);
	bool PathFound = false;
	for (ABallertsCharacter* MyChar : Units)
	{
		if (MyChar)
		{

			float const Distance = FVector::Dist(CurrentDestination, MyChar->GetActorLocation());


			// We need to issue move command only if far enough in order for walk animation to play correctly
			if (NavSys)
			{
				//calculate nav path length for determining the closest pawn which will be the leader
				UNavigationPath * MyPath = NavSys->FindPathToLocationSynchronously(GetWorld(), MyChar->GetActorLocation(), CurrentDestination, MyChar, 0);
				float dist = MyPath->GetPathLength();
				UE_LOG(LogTemp, Warning, TEXT("pathdistance %.2f"), dist);

				if (dist > 0.f && dist < minDistance) {
					minDistance = dist;
					ClosestChar = MyChar;
					PathFound = true;
				}
				//Path->exec
				//NavSys->FindPathToLocationSynchronously(GetWorld(), //NavSys->GetPathLength()
			}
		}
	}
	//if navigation couldnt find a path, then check air distance
	if (Units.Num() > 0 && !PathFound)
	{

		for (ABallertsCharacter* MyChar : Units)
		{
			float dist = FVector::Dist(CurrentDestination, MyChar->GetActorLocation());
			if (dist < minDistance) {
				minDistance = dist;
				ClosestChar = MyChar;
			}

		}
		PathFound = true;
	}
	if (PathFound)
	{
		UE_LOG(LogTemp, Warning, TEXT("leader is: %s"), *(ClosestChar->GetHumanReadableName()));

		//the leader will go to destination, others should follow him
		//NavSys->SimpleMoveToLocation(ClosestChar->GetController(), DestLocation);
		Leader = ClosestChar;

		TArray<ABallertsCharacter*> Followers;
		Followers.Empty(Units.Num() - 1);
		for (int i = 0; i < Units.Num(); i++)
		{
			if (Units[i] != Leader)
				Followers.Add(Units[i]);
		}

		AAIControllerBase* ClosestController = Cast<AAIControllerBase>(ClosestChar->GetController());
		
		ClosestController->SetTargetLocationAsLeader(CurrentDestination, Followers);
		
		for (ABallertsCharacter* MyChar : Followers)
		{
			if (MyChar && MyChar != ClosestChar)
			{
				AAIControllerBase* controller = Cast<AAIControllerBase>(MyChar->GetController());
				//NavSys->SimpleMoveToActor(MyChar->GetController(), ClosestChar);
				if (CurrentShape != EShapeEnum::SE_NONE)
				{
					controller->SetTargetLeader(ClosestChar, ClosestChar->GetActorLocation() - MyChar->GetActorLocation());
				}
				else
				{
					controller->SetTargetActor(ClosestChar);
				}
			}
		}
	}
}

void UUnitGroup::SetWorld_Implementation(UWorld* _World)
{
	World = _World;
}

void UUnitGroup::SetUnits(TArray<ABallertsCharacter*> _Units)
{
	Units = _Units;

	for (int i = 0; i < Units.Num(); i++)
	{
		Units[i]->UnitGroup = this;
	}
}
