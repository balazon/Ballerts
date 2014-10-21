// Fill out your copyright notice in the Description page of Project Settings.

#include "Ballerts.h"
#include "Formation.h"
#include "BalaLib.h"


UFormation::UFormation(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{

}


void UFormation::SetATriangle_Implementation(const FVector2D& Center, float side, const int32 N)
{
	Shapes.Empty();
	UFormationShape* MyShape = NewObject<class UFormationShape>();
	MyShape->SetATriangle(Center, side, N);
	Shapes.Add(MyShape);
}

TArray<FVector2D> UFormation::AllPoints_Implementation()
{
	TArray<FVector2D> Res;
	int ResSize = 0;
	for (int i = 0; i < Shapes.Num(); i++)
	{
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
