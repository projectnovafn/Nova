#pragma once
#include "CoreUObject.h"

class UDataTable : public UObject
{
	GENERATED_UCLASS_BODY(UDataTable, Engine)

public:
	
	template <typename T>
	TMap<FName, T*>& GetAs()
	{
		return *(TMap<FName, T*>*)(&RowMap);
	}

	UStruct* RowStruct;
	TMap<FName, uint8*> RowMap;
};

struct FDataTableRowHandle
{
	GENERATED_USTRUCT_BODY(FDataTableRowHandle, Engine)

public:

	UDataTable* DataTable;
	FName RowName;
};