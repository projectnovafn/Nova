#pragma once
#include "Engine/GameInstance.h"
#include "FortPlaylistManager.h"

class UFortGameInstance : public UGameInstance
{
	GENERATED_UCLASS_BODY(UFortGameInstance, FortniteGame)
public:

	UPROPERTY(UFortPlaylistManager*, PlaylistManager)
	UFortPlaylistManager* PlaylistManager;
};