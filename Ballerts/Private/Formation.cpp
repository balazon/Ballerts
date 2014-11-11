// Fill out your copyright notice in the Description page of Project Settings.

#include "Ballerts.h"
#include "Formation.h"
#include "BalaLib.h"


UFormation::UFormation(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	Shapes.Empty();
}


void UFormation::ClearShapes()
{
	Shapes.Empty();
}

void UFormation::AddTriangle(const FVector2D& Center, float side, const int32 N)
{
	
	Shapes.Add(UFormationShape::CreateTriangle(Center, side, N));
}

//TArray<FVector2D> UFormation::AllPoints_Implementation()
//{
//	TArray<FVector2D> Res;
//	int ResSize = 0;
//	for (int i = 0; i < Shapes.Num(); i++)
//	{
//		ResSize += Shapes[i]->Points.Num();
//	}
//
//	Res.Init(ResSize);
//	int CurrentCount = 0;
//	for (int i = 0; i < Shapes.Num(); i++)
//	{
//
//		for (int j = 0; j < Shapes[i]->Points.Num(); j++)
//		{
//			Res[CurrentCount++] = Shapes[i]->Points[j];
//		}
//	}
//
//	return Res;
//}


void UFormation::BindAllShapesToLeader(ABallertsCharacter* TheLeader)
{
	for (int i = 0; i < Shapes.Num(); i++)
	{
		Shapes[i]->BindToLeader(TheLeader);
	}
}

void UFormation::AssignAllUnits(const TArray<ABallertsCharacter*>& Units)
{
	UFormationShape::AssignShapesToUnits(Shapes, Units);
}
