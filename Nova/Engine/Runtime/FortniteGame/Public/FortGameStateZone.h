#pragma once
#include "FortGameState.h"
#include "Player/FortPlayerState.h"

class AFortCreativePortalManager;

class AFortGameStateZone : public AFortGameState
{
	GENERATED_UCLASS_BODY(AFortGameStateZone, FortniteGame)
public:

	UPROPERTY(AFortCreativePortalManager*, CreativePortalManager)
	AFortCreativePortalManager* CreativePortalManager;

	bool IsRespawningAllowed(AFortPlayerState* PlayerState);
};