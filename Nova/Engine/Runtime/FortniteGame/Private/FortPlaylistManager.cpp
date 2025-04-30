#include "FortPlaylistManager.h"
#include "Engine/GameEngine.h"
#include "Engine/World.h"
#include "FortGameInstance.h"
#include "FortGameMode.h"

UFortPlaylistAthena* UFortPlaylistManager::GetPlaylist(const FName& PlaylistName)
{
	for (auto& Playlist : AthenaPlaylists)
	{
		if (Playlist->PlaylistName == PlaylistName)
		{
			return Playlist;
		}
	}

	return NULL;
}

UFortPlaylistManager* UFortPlaylistManager::Get(UWorld* WorldContext)
{
	auto GameMode = Cast<AFortGameMode>(WorldContext->AuthorityGameMode);
	auto GameInstance = Cast<UFortGameInstance>(WorldContext->GameInstance);

	if (GameMode->GetClassProperty(L"PlaylistManager") != NULL)
		return GameMode->PlaylistManager;

	return GameInstance->PlaylistManager;
}