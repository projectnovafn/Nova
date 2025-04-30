#pragma once
#include "BuildingGameplayActor.h"
#include "Pawns/FortPlayerPawn.h"

class AFortAthenaSupplyDrop : public ABuildingGameplayActor
{
	GENERATED_UCLASS_BODY(AFortAthenaSupplyDrop, FortniteGame)
public:

	UPROPERTY(FName, LootTableNameOverride)
	FName LootTableNameOverride;

	void SpawnLoot(AFortPlayerPawn* Pawn);
};