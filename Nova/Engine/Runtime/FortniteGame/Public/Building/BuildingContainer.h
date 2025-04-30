#pragma once
#include "BuildingSMActor.h"
#include "Pawns/FortPlayerPawn.h"

class FFortSearchBounceData
{
	GENERATED_USTRUCT_BODY(FFortSearchBounceData, FortniteGame)

public:

	UPROPERTY_STRUCT(int, SearchAnimationCount)
	int SearchAnimationCount;
};

class ABuildingContainer : public ABuildingSMActor
{
	GENERATED_UCLASS_BODY(ABuildingContainer, FortniteGame)
public:
	
	UPROPERTY(FFortSearchBounceData, SearchBounceData)
	FFortSearchBounceData SearchBounceData;

	UPROPERTY(FName, SearchLootTierGroup)
	FName SearchLootTierGroup;

	UPROPERTY_BOOL_NOTIFY(bAlreadySearched)
	bool bAlreadySearched;

	void SpawnLoot(AFortPlayerPawn* Pawn);
	void BounceContainer();
};