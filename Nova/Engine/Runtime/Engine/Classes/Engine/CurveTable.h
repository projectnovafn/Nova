#pragma once
#include "CoreUObject.h"

class FCurveTableRowHandle
{
public:
	UObject* CurveTable;
	FName RowName;

	bool IsValid()
	{
		return RowName.ComparisonIndex != 0;
	}
};

class UCurveTable : public UObject
{
	GENERATED_UCLASS_BODY(UCurveTable, Engine)

public:

	template <typename T>
	TMap<FName, T*>& GetAs()
	{
		return *(TMap<FName, T*>*)(&RowMap);
	}

	UStruct* RowStruct;
	TMap<FName, uint8*> RowMap;
};