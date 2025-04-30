#pragma once
#include "CoreUObject.h"
#include "Engine/CurveTable.h"

enum EEvaluateCurveTableResult : uint8
{
	RowFound = 0,
	RowNotFound = 1
};

class UDataTableFunctionLibrary : public UObject
{
	GENERATED_UCLASS_BODY(UDataTableFunctionLibrary, Engine)
public:

	static float EvaluateCurveTableRow(UCurveTable* CurveTable, FName RowName, float InXY);
};