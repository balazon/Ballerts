// Fill out your copyright notice in the Description page of Project Settings.

#include "Ballerts.h"
#include "FormationShape.h"
#include "BalaLib.h"

#include "AIControllerBase.h"



UFormationShape::UFormationShape(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{

}


void UFormationShape::SetATriangle_Implementation(const FVector2D& Center, float side, const int32 N)
{
	Pivot = Center;
	TArray<FVector2D> path;
	path.Init(4);
	path.Empty(4);
	float triEdge = side;

	path.Add(FVector2D(triEdge * 0.5774f, 0.f));
	path.Add(FVector2D(-triEdge * 0.289f, -triEdge * .5f));
	path.Add(FVector2D(-triEdge * 0.289f, triEdge * .5f));
	//path.Add(Center + FVector2D(averageCapDiameter * .5f, triEdge * 0.5774f - averageCapDiameter * 0.866f));



	//get point destinations from triangle edge traversal
	Points = UBalaLib::TraversalPointsOnPath(path, N, true);
}

//index is the index of the point, ShapeIndices mark the starting indices of the shapes in the pointlist
int32 UFormationShape::IndexOfShape(int32 index, const TArray<int32>& ShapeIndices)
{
	int j = 0;
	for (j = 0; j < ShapeIndices.Num(); j++)
	{
		if (ShapeIndices[j] > index)
		{
			break;
		}
	}
	j--;
	return j;
}


void UFormationShape::AssignShapesToUnits(const TArray<UFormationShape*>& Shapes, TArray<ABallertsCharacter*>& Units)
{
	TArray<int> ShapeIndices;
	TArray<FVector2D> AllPoints = UFormationShape::AllPoints(Shapes, ShapeIndices);
	if (AllPoints.Num() != Units.Num())
	{
		UE_LOG(LogTemp, Warning, TEXT("Can assign only equal number of points and units"));
		return;
	}

	//pair the points with the units
	TArray<float> Weights;
	Weights.Init(Units.Num() * Units.Num());
	for (int i = 0; i < Units.Num(); i++)
	{
		ABallertsCharacter* Unit = Units[i];
		FVector Pos = Unit->GetActorLocation();
		FVector2D Loc2D = FVector2D(Pos.X, Pos.Y);
		
		for (int j = 0; j < Units.Num(); j++)
		{
			int index = IndexOfShape(j, ShapeIndices);
			FVector2D offset = Shapes[index].Pivot;
			Weights[i * Units.Num() + j] = -FVector2D::Distance(Loc2D, AllPoints[j] + offset);
		}
	}
	TArray<int32> indexAssigns;
	UBalaLib::Assignment(Weights, Units.Num(), indexAssigns);
	//Setting shapes for respective controllers
	for (int32 i = 0; i < Units.Num(); i++)
	{
		ABallertsCharacter* Unit = Units[i];
		if (Unit)
		{
			
			AAIControllerBase* Controller = Cast<AAIControllerBase>(Unit->GetController());
			int j = IndexOfShape(indexAssigns[i], ShapeIndices);
			
			Controller->SetShape(Shapes[ShapeIndices[j]]);
			Shapes[ShapeIndices[j]]->Assignments[Controller] = indexAssigns[i] - ShapeIndices[j];
			//UE_LOG(LogTemp, Warning, TEXT("Path: %.2f %.2f"), res[indexAssigns[i]].X, res[indexAssigns[i]].Y);
		}
	}
}

TArray<FVector2D> UFormationShape::AllPoints(const TArray<UFormationShape*>& Shapes, TArray<int>& ShapeIndices)
{
	TArray<FVector2D> Res;
	int ResSize = 0;
	ShapeIndices.Empty();
	for (int i = 0; i < Shapes.Num(); i++)
	{
		if (i == 0 || i != Shapes.Num() - 1)
		{
			ShapeIndices.Add(ResSize);
		}
		ResSize += Shapes[i]->Points.Num();
	}

	Res.Init(ResSize);
	int CurrentCount = 0;
	for (int i = 0; i < Shapes.Num(); i++)
	{

		for (int j = 0; j < Shapes[i]->Points.Num(); j++)
		{
			Res[CurrentCount++] = Shapes[i]->Points[j];
		}
	}

	return Res;
}

FVector2D UFormationShape::TransformedPoint(AAIControllerBase* Controller)
{
	int index = Assignments[Controller];
	
	//should later transform point according to rotation, and other transformations
	return Points[index] + Pivot;
}


static UFormationShape* CreateTriangle(const FVector2D& Center, float side, const int32 N)
{
	UFormationShape* MyShape = NewObject<class UFormationShape>();
	MyShape->SetATriangle(Center, side, N);

	return MyShape;
}