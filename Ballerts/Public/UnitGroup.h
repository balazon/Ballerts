// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"

#include "BallertsCharacter.h"
#include "Formation.h"

#include "UnitGroup.generated.h"


UENUM(BlueprintType)
namespace EShapeEnum
{
	enum Type
	{
		SE_TRIANGLE,
		SE_SQUARE,
		SE_NONE
	};
}

USTRUCT(BlueprintType)
struct FUnitStruct
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	FVector2D AssignedPoint;

	FUnitStruct()
	{
		AssignedPoint = FVector2D::ZeroVector;
	}
};

/**
 * 
 */
UCLASS()
class BALLERTS_API UUnitGroup : public UObject
{
	GENERATED_UCLASS_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = UnitGroup)
	TArray<ABallertsCharacter*> Units;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = UnitGroup)
	ABallertsCharacter* Leader;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = UnitGroup)
	FVector CurrentDestination;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UnitGroup)
	TEnumAsByte<EShapeEnum::Type> CurrentShape;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = UnitGroup)
	UFormation* Formation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = UnitGroup)
	TMap<ABallertsCharacter*, FUnitStruct> UnitData;

	UFUNCTION(BlueprintNativeEvent, Category = UnitGroup)
	void MoveToFormation(const TEnumAsByte<EShapeEnum::Type>& ShapeType);

	UFUNCTION(BlueprintNativeEvent, Category = UnitGroup)
	void SetDestination(const FVector& Destination);

	UFUNCTION(BlueprintNativeEvent, Category = UnitGroup)
	void SetWorld(UWorld* _World);

	UFUNCTION(Category = UnitGroup)
	void SetUnits(TArray<ABallertsCharacter*> _Units);

	UFUNCTION(Category = UnitGroup)
	int Num();

	UFUNCTION(Category = UnitGroup)
	void AddUnit(ABallertsCharacter* Unit);

	UFUNCTION(Category = UnitGroup)
	void RemoveUnit(ABallertsCharacter* Unit);

protected:
	UWorld* World;

	UFUNCTION(BlueprintNativeEvent, Category = UnitGroup)
	void MoveToFormationTriangle();

};
