#pragma once

#include "CoreUObject.h"

#include "Engine/DataTable.h"
#include "Engine/DataAsset.h"

#include "ItemDefinitions/FortGameplayModifierItemDefinition.h"

enum class EAthenaRespawnType : uint8
{
	None = 0,
	InfiniteRespawn = 1,
	InfiniteRespawnExceptStorm = 2
};

class UFortPlaylist : public UPrimaryDataAsset
{
	GENERATED_UCLASS_BODY(UFortPlaylist, FortniteGame)
public:

	UPROPERTY(FName, PlaylistName)
	FName PlaylistName;

	UPROPERTY(int, MaxPlayers)
	int MaxPlayers;

	UPROPERTY(TArray<TSoftObjectPtr<UWorld>>, AdditionalLevels)
	TArray<TSoftObjectPtr<UWorld>> AdditionalLevels;

	UPROPERTY(TSoftObjectPtr<UDataTable>, LootTierData)
	TSoftObjectPtr<UDataTable> LootTierData;

	UPROPERTY(TSoftObjectPtr<UDataTable>, LootPackages)
	TSoftObjectPtr<UDataTable> LootPackages;

	UPROPERTY(TArray<TSoftObjectPtr<UFortGameplayModifierItemDefinition>>, ModifierList)
	TArray<TSoftObjectPtr<UFortGameplayModifierItemDefinition>> ModifierList;
};

class UFortPlaylistAthena : public UFortPlaylist
{
	GENERATED_UCLASS_BODY(UFortPlaylistAthena, FortniteGame)
public:

	UPROPERTY(EAthenaRespawnType, RespawnType)
	EAthenaRespawnType RespawnType;
};