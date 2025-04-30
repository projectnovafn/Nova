#pragma once
#include "FortPersistableItem.h"
#include "Mcp\McpProfile.h"

#include "FortCreativeRealEstatePlotItemDefinition.h"

enum class EFortCreativePlotPermission : uint8
{
	Private = 0,
	Public = 1
};

class FFortCreativePlotPermissionData
{
	GENERATED_USTRUCT_BODY(FFortCreativePlotPermissionData, FortniteGame)
public:

	UPROPERTY_STRUCT(EFortCreativePlotPermission, Permission)
	EFortCreativePlotPermission Permission;

	UPROPERTY_STRUCT(TArray<FString>, WhiteList)
	TArray<FString> WhiteList;
};

class UFortCreativeRealEstatePlotItem : public UFortPersistableItem
{
	GENERATED_UCLASS_BODY(UFortCreativeRealEstatePlotItem, FortniteGame)
public:

	UPROPERTY(int, IslandIndex)
	int IslandIndex;

	UPROPERTY(FString, IslandInfoLocale)
	FString IslandInfoLocale;

	UPROPERTY(FString, IslandTitle)
	FString IslandTitle;

	UPROPERTY(FString, IslandTagline)
	FString IslandTagline;

	UPROPERTY(FString, IslandCode)
	FString IslandCode;

	UPROPERTY_BOOL(bIsPromoted)
	bool bIsPromoted;

	UPROPERTY(FFortCreativePlotPermissionData, PermissionData)
	FFortCreativePlotPermissionData PermissionData;

	UPROPERTY(FDateTime, DeletedAt)
	FDateTime DeletedAt;

	UPROPERTY(FDateTime, LastUsedDate)
	FDateTime LastUsedDate;

	void ParseAttributes(FMcpItem* McpItem);
};