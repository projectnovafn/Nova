#include "Online/FortGameSessionDedicated.h"
#include "Athena/FortGameModeAthena.h"
#include "Kismet/KismetArrayLibrary.h"
#include "Nova.h"

void AFortGameSessionDedicated::HandleMatchAssignment(FMatchmakingDedicatedServerMatchAssignment& Assignment)
{
	GNova->bHasStartedZone = false;
	GNova->bHasSetZonePhase = false;
	GNova->bHasSetupBus = false;

	GNova->bIsLateGame = Assignment.PlaylistName == L"Playlist_Lite_Solo" ? Assignment.PlaylistName = L"Playlist_DefaultSolo", true :
		Assignment.PlaylistName == L"Playlist_Lite_Duos" ? Assignment.PlaylistName = L"Playlist_DefaultDuo", true :
		Assignment.PlaylistName == L"Playlist_Lite_Squads" ? Assignment.PlaylistName = L"Playlist_DefaultSquad", true : false;

	auto PlaylistManager = UFortPlaylistManager::Get(GetWorld());
	auto Playlist = PlaylistManager->GetPlaylist(Assignment.PlaylistName);

	UE_LOG(LogOnlineGame, Log, TEXT("HandleMatchAssignment: %s"), *GetName());

	if (Playlist != NULL)
	{
		auto GameMode = Cast<AFortGameModeAthena>(GetWorld()->AuthorityGameMode);
		auto GameState = Cast<AFortGameStateAthena>(GetWorld()->GameState);

		GameState->SetCurrentPlaylist(Playlist);
		
		for (auto& Level : Playlist->AdditionalLevels)
		{
			auto LevelName = Level.ToSoftObjectPath().GetAssetPathName();

			UKismetArrayLibrary::Array_Add(GameState->AdditionalPlaylistLevelsStreamed, GameState->GetClassProperty(L"AdditionalPlaylistLevelsStreamed"), LevelName);
		}

		GameState->OnRep_AdditionalPlaylistLevelsStreamed();

		MaxPlayers = Playlist->MaxPlayers;
	}
}

void AFortGameSessionDedicated::Restart()
{
	Sleep(4500);

	FPlatformMisc::RequestExit(true);
}