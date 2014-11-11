// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"


#include "FormationShape.h"
#include "Formation.generated.h"




/**
 * 
 */
UCLASS()
class BALLERTS_API UFormation : public UObject
{
	GENERATED_UCLASS_BODY()


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Formation)
	TArray<UFormationShape*> Shapes;

	UFUNCTION(BlueprintCallable, Category = Formation)
	void ClearShapes();

	UFUNCTION(BlueprintCallable, Category = Formation)
	void AddTriangle(const FVector2D& Center, float side, const int32 N);

	/*UFUNCTION(BlueprintNativeEvent, Category = Formation)
	TArray<FVector2D> AllPoints();*/


	UFUNCTION(BlueprintCallable, Category = Formation)
	void AssignAllUnits(const TArray<ABallertsCharacter*>& Units);

	UFUNCTION(BlueprintCallable, Category = Formation)
	void BindAllShapesToLeader(ABallertsCharacter* TheLeader);
	

	
};
