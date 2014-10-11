// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "Ballerts.h"
#include "BallertsPlayerController.h"
#include "AI/Navigation/NavigationSystem.h"
#include "AI/Navigation/NavigationPath.h"
#include "math.h"

#include "BallertsCharacter.h"
#include "GameFramework/HUD.h"


#define SELECTION_INITSIZE 10


ABallertsPlayerController::ABallertsPlayerController(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Crosshairs;

	SelectedUnits.Init(SELECTION_INITSIZE);
	SelectedUnits.Empty(SELECTION_INITSIZE);

	Delta = 0.f;

	

}

void ABallertsPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	// keep updating the destination every tick while desired
	if (bMoveToMouseCursor)
	{
		MoveToMouseCursor();
	}

	Delta = DeltaTime;
}

void ABallertsPlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

	InputComponent->BindAction("SetDestination", IE_Pressed, this, &ABallertsPlayerController::OnSetDestinationPressed);
	InputComponent->BindAction("SetDestination", IE_Released, this, &ABallertsPlayerController::OnSetDestinationReleased);

	// support touch devices 
	InputComponent->BindTouch(EInputEvent::IE_Pressed, this, &ABallertsPlayerController::MoveToTouchLocation);
	InputComponent->BindTouch(EInputEvent::IE_Repeat, this, &ABallertsPlayerController::MoveToTouchLocation);

	InputComponent->BindAction("Click", IE_Pressed, this, &ABallertsPlayerController::OnClickPressed);
	InputComponent->BindAction("Click", IE_Released, this, &ABallertsPlayerController::OnClickReleased);

	InputComponent->BindAction("IncreaseSelection", IE_Pressed, this, &ABallertsPlayerController::OnIncreaseSelectionPressed);
	InputComponent->BindAction("IncreaseSelection", IE_Released, this, &ABallertsPlayerController::OnIncreaseSelectionReleased);

	InputComponent->BindAction("Zoom", IE_Pressed, this, &ABallertsPlayerController::OnZoomStart);

	InputComponent->BindAction("Zoom", IE_Released, this, &ABallertsPlayerController::OnZoomProcessing);

	//InputComponent->BindAction("MoveForward", IE_Pressed, this, &ABallertsPlayerController::OnPanVerticalStart);

	//InputComponent->BindAction("MoveRight", IE_Pressed, this, &ABallertsPlayerController::OnPanHorizontalStart);

	InputComponent->BindAxis("MoveRight", this, &ABallertsPlayerController::OnPanVerticalStart);

	InputComponent->BindAxis("MoveForward", this, &ABallertsPlayerController::OnPanHorizontalStart);

	InputComponent->BindAction("RightClick", IE_Pressed, this, &ABallertsPlayerController::OnRightClickPressed);


}

void ABallertsPlayerController::BeginPlay()
{
	Super::BeginPlay();

	AHUD* Hud = GetHUD();
	bEnableClickEvents = true;
	bShowMouseCursor = true;
	Hud->EnableInput(this);
	
}

void ABallertsPlayerController::MoveToMouseCursor()
{
	// Trace to see what is under the mouse cursor
	FHitResult Hit;
	GetHitResultUnderCursor(ECC_Visibility, false, Hit);

	if (Hit.bBlockingHit)
	{
		// We hit something, move there
		SetNewMoveDestination(Hit.ImpactPoint);
	}
}

void ABallertsPlayerController::MoveToTouchLocation(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	FVector2D ScreenSpaceLocation(Location);

	// Trace to see what is under the touch location
	FHitResult HitResult;
	GetHitResultAtScreenPosition(ScreenSpaceLocation, CurrentClickTraceChannel, true, HitResult);
	if (HitResult.bBlockingHit)
	{
		// We hit something, move there
		SetNewMoveDestination(HitResult.ImpactPoint);
	}
}

void ABallertsPlayerController::SetNewMoveDestination(const FVector DestLocation)
{
	float minDistance = 10000.f;
	ABallertsCharacter* ClosestChar = NULL;
	UNavigationSystem* const NavSys = GetWorld()->GetNavigationSystem();

	//UE_LOG(LogTemp, Warning, TEXT("%.3f"), DestLocation.Z);
	bool PathFound = false;
	for (ABallertsCharacter* MyChar : SelectedUnits)
	{
		if (MyChar)
		{
			
			float const Distance = FVector::Dist(DestLocation, MyChar->GetActorLocation());
			

			// We need to issue move command only if far enough in order for walk animation to play correctly
			if (NavSys && (Distance > 10.0f))
			{
				//calculate nav path length for determining the closest pawn which will be the leader
				UNavigationPath * MyPath = NavSys->FindPathToLocationSynchronously(GetWorld(), MyChar->GetActorLocation(), DestLocation, MyChar, 0);
				float dist = MyPath->GetPathLenght();
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

	if (PathFound && SelectedUnits.Num() > 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("leader is: %s"), *(ClosestChar->GetHumanReadableName()));

		//the leader will go to destination, others should follow him
		NavSys->SimpleMoveToLocation(ClosestChar->GetController(), DestLocation);

		for (ABallertsCharacter* MyChar : SelectedUnits)
		{
			if (MyChar && MyChar != ClosestChar)
			{
				NavSys->SimpleMoveToActor(MyChar->GetController(), ClosestChar);
				//NavSys->SimpleMoveToLocation(MyChar->GetController(), DestLocation);

			}

		}
	}
}

void ABallertsPlayerController::MoveToFormation()
{
	UE_LOG(LogTemp, Warning, TEXT("FORMATION"));

	//calculate the triangle's shape
	FVector2D Center = FVector2D(0.f, 0.f);
	int count = 0;
	float sumCapDiameter = 0.f;
	for (ABallertsCharacter* MyChar : SelectedUnits)
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

	TArray<FVector2D> path;
	path.Init(4);
	path.Empty(4);

	float averageCapDiameter = sumCapDiameter / count;
	path.Add(Center + FVector2D(triEdge * 0.5774f, 0.f ));
	path.Add(Center + FVector2D(-triEdge * 0.289f, -triEdge * .5f));
	path.Add(Center + FVector2D(-triEdge * 0.289f, triEdge * .5f));
	//path.Add(Center + FVector2D(averageCapDiameter * .5f, triEdge * 0.5774f - averageCapDiameter * 0.866f));



	//get point destinations from triangle edge traversal

	TArray<FVector2D> res = TraversalPointsOnPath(path, count, true);

	CurrentTargets.Init(count);
	CurrentTargets.Empty(count);
	//issue commands to move there
	for (int32 i = 0; i < SelectedUnits.Num(); i++)
	{
		ABallertsCharacter* MyChar = SelectedUnits[i];
		if (MyChar)
		{
			UNavigationSystem* NavSys = GetWorld()->GetNavigationSystem();
			NavSys->SimpleMoveToLocation(MyChar->GetController(), FVector(res[i],120.f));
			UE_LOG(LogTemp, Warning, TEXT("Path: %.2f %.2f"), res[i].X, res[i].Y);
			CurrentTargets.Add(FVector(res[i], 120.f));
			

		}
	}
}

void ABallertsPlayerController::OnSetDestinationPressed()
{
	// set flag to keep updating destination until released
	//bMoveToMouseCursor = true;
}

void ABallertsPlayerController::OnSetDestinationReleased()
{
	// clear flag to indicate we should stop updating the destination
	//bMoveToMouseCursor = false;
}

void ABallertsPlayerController::DeselectAll()
{
	UE_LOG(LogTemp, Warning, TEXT("DESELECT ALL"));
	for (ABallertsCharacter* Unit : SelectedUnits)
	{
		if (Unit)
		{
			Unit->SelectedEffect->SetVisibility(false);
		}
	}
	SelectedUnits.Init(SELECTION_INITSIZE);
	SelectedUnits.Empty(SELECTION_INITSIZE);

}

void ABallertsPlayerController::OnClickPressed()
{
	UE_LOG(LogTemp, Warning, TEXT("clickpressed"));
	FHitResult Hit;
	GetHitResultUnderCursor(ECC_Visibility, false, Hit);
	FVector2D MouseLocation;
	GetMousePosition(MouseLocation.X, MouseLocation.Y);

	int32 ViewportX;
	int32 ViewportY;
	GetViewportSize(ViewportX, ViewportY);

	if (MouseLocation.X > ViewportX * .9f)// && MouseLocation.Y > ViewportY * .9f)
	{
		//DeselectAll();
		
		return;
	}
	
	/*if (MouseLocation.X > ViewportX * .9f && MouseLocation.Y > ViewportY * .4f && MouseLocation.Y < ViewportY * .6f)
	{
		MoveToFormation();

		return;
	}*/

	if (Hit.bBlockingHit)
	{
		UE_LOG(LogTemp, Warning, TEXT("clickedsomething"));


		APawn* pawn = Cast<APawn>(Hit.GetActor());
		if (pawn != NULL)
		{
			UE_LOG(LogTemp, Warning, TEXT("clicked pawn"));

			ABallertsCharacter* Unit = Cast<ABallertsCharacter>(pawn);
			if (Unit != NULL)
			{
				//deselect the clicked unit
				if (SelectedUnits.Contains(Unit))
				{
					RemoveUnitFromSelection(Unit);
				}
				//add unit to selection
				else
				{
					AddUnitToSelection(Unit);
				}
				
			}
		}
		else
		{
			//give move command to move
			UE_LOG(LogTemp, Warning, TEXT("MOVE COMMAND_A"));
			MoveToMouseCursor();
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("MOVE COMMAND_B"));
		MoveToMouseCursor();
	}
}

void ABallertsPlayerController::OnClickReleased()
{
	
}

void ABallertsPlayerController::OnIncreaseSelectionPressed()
{
	UE_LOG(LogTemp, Warning, TEXT("shiftclickpressed"));

	FHitResult Hit;
	GetHitResultUnderCursor(ECC_Visibility, false, Hit);

	if (Hit.bBlockingHit)
	{
		APawn* pawn = Cast<APawn>(Hit.Actor.Get());
		if (pawn != NULL)
		{
			ABallertsCharacter* Unit = Cast<ABallertsCharacter>(pawn);
			if (Unit != NULL)
			{
				SelectedUnits.Add(Unit);
				Unit->SelectedEffect->SetVisibility(true);
			}
		}
	}
}

void ABallertsPlayerController::OnIncreaseSelectionReleased()
{

}


void ABallertsPlayerController::OnZoomStart()
{

}

void ABallertsPlayerController::OnZoomProcessing()
{

}

void ABallertsPlayerController::OnPanVerticalStart(float value)
{
	//UE_LOG(LogTemp, Warning, TEXT("RIGHT -- LEFT"));
	
	GetPawn()->AddMovementInput(FVector(0,10.f, 0), value);
	if (true)
	{
		GetPawn()->AddActorLocalOffset(FVector(0, 500.f * value * Delta, 0));
		//UE_LOG(LogTemp, Warning, TEXT("RIGHT -- LEFT"));

	}
	//SetActorLocation()
}

void ABallertsPlayerController::OnPanVerticalProcessing(float value)
{

}


void ABallertsPlayerController::OnPanHorizontalStart(float value)
{
	GetPawn()->AddMovementInput(FVector(10.f, 0, 0), value);
	//FMath::Abs(value) > .1f
	if (true)
	{
		GetPawn()->AddActorLocalOffset(FVector(500.f * value * Delta, 0, 0));
		//UE_LOG(LogTemp, Warning, TEXT("RIGHT -- LEFT"));

	}
}

void ABallertsPlayerController::OnPanHorizontalProcessing(float value)
{
	
}


//places a point to the start, and the end, and between the vertexes, with the neighbouring vertexes having equal distance
//closed should be set to true, if the path is a closed loop
TArray<FVector2D> ABallertsPlayerController::TraversalPointsOnPath(TArray<FVector2D> path, int pointCount, bool closed = false)
{
	if (closed)
	{
		FVector2D first = path[0];
		path.Add(first);
	}
	float pathLength = 0;
	FVector2D previousPoint;
	bool first = true;
	TArray<float> AccumulatedDist;
	AccumulatedDist.Init(path.Num()-1);
	AccumulatedDist.Empty(path.Num() - 1);

	for (FVector2D point : path)
	{
		if (!first)
		{
			float d = FVector2D::Distance(point, previousPoint);
			pathLength += d;
			AccumulatedDist.Add(pathLength);
		}
		previousPoint = point;
		first = false;
	}

	float pointDistance = pathLength / (float)(pointCount - 1);
	if (closed)
	{
		pointDistance = pathLength / (float)(pointCount);

	}

	TArray<FVector2D> result;
	result.Init(pointCount);
	result.Empty(pointCount);

	float currentDist = 0.f;
	int j = 0;
	for (int i = 0; i < pointCount; i++)
	{
		while (currentDist > AccumulatedDist[j])
		{
			j++;
		}
		//interpolating between two points
		float distBefore = 0.f;
		if (j != 0) distBefore = AccumulatedDist[j - 1];
		float p = (AccumulatedDist[j] - currentDist) / (AccumulatedDist[j] - distBefore);
		FVector2D point = path[j] * p + path[j + 1] * (1 - p);
		result.Add(point);
		currentDist += pointDistance;
	}
	return result;
}

void ABallertsPlayerController::OnRightClickPressed()
{
	//testing traversalpoints:

	TArray<FVector2D> path;
	path.Init(4);
	path.Empty(4);
	path.Add(FVector2D(0.f, 1.f));
	path.Add(FVector2D(0.f, 0.f));
	path.Add(FVector2D(1.f, 0.f));
	path.Add(FVector2D(1.f, 1.f));


	TArray<FVector2D> res = TraversalPointsOnPath(path, 8, true);

	for (FVector2D p : res)
	{
		UE_LOG(LogTemp, Warning, TEXT("%.3f %.3f\n"), p.X, p.Y);

	}
}


TArray<FVector> ABallertsPlayerController::GetTargets()
{
	return CurrentTargets;
}

void ABallertsPlayerController::AddUnitToSelection(ABallertsCharacter* Unit)
{
	SelectedUnits.Add(Unit);
	Unit->SelectedEffect->SetVisibility(true);
}

void ABallertsPlayerController::RemoveUnitFromSelection(ABallertsCharacter* Unit)
{
	SelectedUnits.Remove(Unit);
	Unit->SelectedEffect->SetVisibility(false);
}