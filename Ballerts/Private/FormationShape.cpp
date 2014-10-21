// Fill out your copyright notice in the Description page of Project Settings.

#include "Ballerts.h"
#include "FormationShape.h"
#include "BalaLib.h"



UFormationShape::UFormationShape(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{

}


void UFormationShape::SetATriangle_Implementation(const FVector2D& Center, float side, const int32 N)
{
	TArray<FVector2D> path;
	path.Init(4);
	path.Empty(4);
	float triEdge = side;

	path.Add(Center + FVector2D(triEdge * 0.5774f, 0.f));
	path.Add(Center + FVector2D(-triEdge * 0.289f, -triEdge * .5f));
	path.Add(Center + FVector2D(-triEdge * 0.289f, triEdge * .5f));
	//path.Add(Center + FVector2D(averageCapDiameter * .5f, triEdge * 0.5774f - averageCapDiameter * 0.866f));



	//get point destinations from triangle edge traversal
	Points = UBalaLib::TraversalPointsOnPath(path, N, true);
}