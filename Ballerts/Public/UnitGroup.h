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

	UFUNCTION(BlueprintNativeEvent, Category = UnitGroup)
	void MoveToFormation(const TEnumAsByte<EShapeEnum::Type>& ShapeType);

	UFUNCTION(BlueprintNativeEvent, Category = UnitGroup)
	void SetDestination(const FVector& Destination);

	UFUNCTION(BlueprintNativeEvent, Category = UnitGroup)
	void SetWorld(UWorld* _World);

	UFUNCTION(Category = UnitGroup)
	void SetUnits(TArray<ABallertsCharacter*> _Units);

protected:
	UWorld* World;

	UFUNCTION(BlueprintNativeEvent, Category = UnitGroup)
	void MoveToFormationTriangle();
};
