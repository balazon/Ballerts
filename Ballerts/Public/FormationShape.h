// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AIControllerBase.h"

#include "Object.h"
#include "FormationShape.generated.h"

/**
 * 
 */
UCLASS()
class BALLERTS_API UFormationShape : public UObject
{
	GENERATED_UCLASS_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Formation)
	TArray<FVector2D> Points;


	UFUNCTION(BlueprintCallable, Category = Formation)
	void SetATriangle(const FVector2D& Center, float side, const int32 N);

	UFUNCTION(BlueprintCallable, Category = Formation)
	void Rotate(float angle);

	UFUNCTION(BlueprintCallable, Category = Formation)
	FVector2D TransformedPoint(AAIControllerBase* Controller);
	
	UFUNCTION(BlueprintCallable, Category = Formation)
	void BindToLeader(ABallertsCharacter* TheLeader);



	UFUNCTION(BlueprintCallable, Category = Formation)
	static UFormationShape* CreateTriangle(const FVector2D& Center, float side, const int32 N);

	UFUNCTION(BlueprintCallable, Category = Formation)
	static void AssignShapesToUnits(const TArray<UFormationShape*>& Shapes, const TArray<ABallertsCharacter*>& Units);

	UFUNCTION(BlueprintCallable, Category = Formation)
	static TArray<FVector2D> AllPoints(const TArray<UFormationShape*>& Shapes, TArray<int32>& ShapeIndices);

protected:

	UPROPERTY()
	float rotationAngle;

	UPROPERTY()
	FVector2D Pivot;

	TMap<AAIControllerBase*, int32> Assignments;

	UPROPERTY()
	ABallertsCharacter* Leader;

	UPROPERTY()
	FVector2D LeaderStart;

	UFUNCTION(BlueprintCallable, Category = Formation)
	static int32 IndexOfShape(int32 index, const TArray<int32>& ShapeIndices);


};
