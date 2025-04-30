#pragma once
#include "FortGameModeBase.h"
#include "Engine.h"

class UFortPlaylistManager;

class AFortGameMode : public AFortGameModeBase
{
	GENERATED_UCLASS_BODY(AFortGameMode, FortniteGame)
public:

	UPROPERTY_BOOL(bWorldIsReady)
	bool bWorldIsReady;

	UPROPERTY(UFortPlaylistManager*, PlaylistManager)
	UFortPlaylistManager* PlaylistManager;
};