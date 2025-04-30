#include "Athena/FortGameStateAthena.h"
#include "Kismet/FortKismetLibrary.h"

#include "Kismet/KismetSystemLibrary.h"

AFortAthenaAircraft* AFortGameStateAthena::GetAircraft(int AircraftIndex)
{
	struct
	{
		int AircraftIndex;
		AFortAthenaAircraft* ReturnValue;
	} params(AircraftIndex);

	this->ProcessEvent(L"GetAircraft", &params);

	return params.ReturnValue;
}

int AFortGameStateAthena::GetAliveTeams()
{
	int Place = 0;

	for (auto& Team : Teams)
	{
		if (Team->IsAlive())
			Place++;
	}

	return Place;
}

void AFortGameStateAthena::OnRep_GamePhase(EAthenaGamePhase OldGamePhase)
{
	this->ProcessEvent(L"OnRep_GamePhase", &OldGamePhase);
}

void AFortGameStateAthena::OnRep_CurrentPlaylistData()
{
	this->ProcessEvent(L"OnRep_CurrentPlaylistData");
}

void AFortGameStateAthena::OnRep_PlayersLeft()
{
	this->ProcessEvent(L"OnRep_PlayersLeft");
}

bool AFortGameStateAthena::IsGameInitialized()
{
	return GetCurrentPlaylist() != NULL;
}

UFortPlaylistAthena* AFortGameStateAthena::GetCurrentPlaylist()
{
	static auto CurrentPlaylistInfoProp = this->GetClassProperty(L"CurrentPlaylistInfo");

	if (CurrentPlaylistInfoProp != NULL)
		return CurrentPlaylistInfo.BasePlaylist;
	else
		return CurrentPlaylistData;
}

void AFortGameStateAthena::SetCurrentPlaylist(UFortPlaylistAthena* Playlist)
{
	static auto CurrentPlaylistInfoProp = this->GetClassProperty(L"CurrentPlaylistInfo");

	if (CurrentPlaylistInfoProp != NULL)
	{
		CurrentPlaylistInfo.BasePlaylist = Playlist;
		CurrentPlaylistInfo.PlaylistReplicationKey++;
		CurrentPlaylistInfo.MarkArrayDirty();

		OnRep_CurrentPlaylistInfo();
	}
	else
	{
		CurrentPlaylistData = Playlist;

		OnRep_CurrentPlaylistData();
	}
}

TArray<TWeakObjectPtr<AFortPlayerStateAthena>>* AFortGameStateAthena::FindTeamMembers(uint8 TeamIndex)
{
	static TArray<TWeakObjectPtr<AFortPlayerStateAthena>> Teams[100];
	auto& TeamMembers = Teams[TeamIndex % 100];

	TeamMembers.Empty();

	for (auto Player : PlayerArray)
	{
		auto PlayerState = Cast<AFortPlayerStateAthena>(Player);

		if (!PlayerState || PlayerState->TeamIndex != TeamIndex) continue;

		TeamMembers.Add(PlayerState);
	}

	return &TeamMembers;
}

TArray<TWeakObjectPtr<AFortPlayerStateAthena>>* AFortGameStateAthena::FindSquadMembers(uint8 SquadId)
{
	static TArray<TWeakObjectPtr<AFortPlayerStateAthena>> Squads[100];
	auto& SquadMembers = Squads[SquadId % 100];

	SquadMembers.Empty();

	for (auto Player : PlayerArray)
	{
		auto PlayerState = Cast<AFortPlayerStateAthena>(Player);

		if (!PlayerState || PlayerState->SquadId != SquadId) continue;

		SquadMembers.Add(PlayerState);
	}

	return &SquadMembers;
}

bool AFortGameStateAthena::IsResurrectionEnabled(AFortPlayerPawn* Pawn)
{
	UE_LOG(LogFort, Log, TEXT("IsResurrectionEnabled"));

	if (UKismetSystemLibrary::GetFortniteVersion() == 10.40)
		return GetCurrentPlaylist() && GetCurrentPlaylist()->RespawnType == EAthenaRespawnType::None;

	return false;
}

FGameplayTagContainer AFortGameStateAthena::GetAthenaPlaylistContextTags()
{
	FGameplayTagContainer Tags;
	this->ProcessEvent(L"GetAthenaPlaylistContextTags", &Tags);

	return Tags;
}