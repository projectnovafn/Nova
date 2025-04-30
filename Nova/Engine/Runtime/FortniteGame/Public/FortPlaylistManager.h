#pragma once
#include "CoreUObject.h"
#include "FortPlaylist.h"

class UFortPlaylistManager : public UObject
{
	GENERATED_UCLASS_BODY(UFortPlaylistManager, FortniteGame)
public:

	TArray<UFortPlaylistAthena*> AthenaPlaylists;

	UFortPlaylistAthena* GetPlaylist(const FName& PlaylistName);

	static UFortPlaylistManager* Get(UWorld* WorldContext);
};