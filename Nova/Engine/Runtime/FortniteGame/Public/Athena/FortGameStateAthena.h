#pragma once
#include "FortGameStatePvP.h"
#include "FortPlaylist.h"
#include "Engine/NetSerialization.h"

#include "FortVolumeManager.h"

#include "Athena/FortAthenaAircraft.h"
#include "Athena/Player/FortPlayerStateAthena.h"

enum class EAthenaGamePhase : uint8
{
	None = 0,
	Setup = 1,
	Warmup = 2,
	Aircraft = 3,
	SafeZones = 4,
	EndGame = 5,
	Count = 6
};

class FPlaylistPropertyArray : public FFastArraySerializer
{
	GENERATED_USTRUCT_BODY(FPlaylistPropertyArray, FortniteGame)
public:

	UPROPERTY_STRUCT(UFortPlaylistAthena*, BasePlaylist)
	UFortPlaylistAthena* BasePlaylist;

	UPROPERTY_STRUCT(int32, PlaylistReplicationKey)
	int32 PlaylistReplicationKey;
};

class FGameMemberInfo : public FFastArraySerializerItem
{
	GENERATED_USTRUCT_BODY(FGameMemberInfo, FortniteGame)
	USTRUCT_COPYABLE(FGameMemberInfo)

public:

	UPROPERTY_STRUCT(uint8, TeamIndex)
	uint8 TeamIndex;

	UPROPERTY_STRUCT(uint8, SquadId)
	uint8 SquadId;

	UPROPERTY_STRUCT_PTR(FUniqueNetIdRepl, MemberUniqueId)
	FUniqueNetIdRepl* MemberUniqueId;

	static FGameMemberInfo& Construct(uint8 InTeamIndex, uint8 InSquadId, FUniqueNetIdRepl* UniqueId)
	{
		auto MemberInfo = FGameMemberInfo::Allocate();

		MemberInfo->ReplicationID = -1;
		MemberInfo->ReplicationKey = -1;
		MemberInfo->MostRecentArrayReplicationKey = -1;

		MemberInfo->SquadId = InSquadId;
		MemberInfo->TeamIndex = InTeamIndex;
		MemberInfo->MemberUniqueId = UniqueId;

		return *MemberInfo;
	}
};

class FGameMemberInfoArray : public FFastArraySerializer
{
	GENERATED_USTRUCT_BODY(FGameMemberInfoArray, FortniteGame)

public:

	UPROPERTY_STRUCT(TArray<FGameMemberInfo>, Members)
	TArray<FGameMemberInfo> Members;

	UPROPERTY_STRUCT(class AFortGameStateAthena*, OwningGameState)
	class AFortGameStateAthena* OwningGameState;
};

class AFortGameStateAthena : public AFortGameStatePvP
{
	GENERATED_UCLASS_BODY(AFortGameStateAthena, FortniteGame)
public:

	UPROPERTY_NOTIFY(FPlaylistPropertyArray, CurrentPlaylistInfo)
	FPlaylistPropertyArray CurrentPlaylistInfo;

	UPROPERTY(UFortPlaylistAthena*, CurrentPlaylistData)
	UFortPlaylistAthena* CurrentPlaylistData;

	UPROPERTY(UObject*, MapInfo)
	UObject* MapInfo;

	UPROPERTY(AFortVolumeManager*, VolumeManager)
	AFortVolumeManager* VolumeManager;

	UPROPERTY(APlayerState*, WinningPlayerState)
	APlayerState* WinningPlayerState;

	UPROPERTY(FGameMemberInfoArray, GameMemberInfoArray)
	FGameMemberInfoArray GameMemberInfoArray;

	UPROPERTY(EAthenaGamePhase, GamePhase)
	EAthenaGamePhase GamePhase;

	UPROPERTY(uint8, SafeZonePhase)
	uint8 SafeZonePhase;

	UPROPERTY_BOOL(bAircraftIsLocked)
	bool bAircraftIsLocked;

	UPROPERTY(int, PlayersLeft)
	int PlayersLeft;

	UPROPERTY(int, TotalPlayers)
	int TotalPlayers;

	UPROPERTY(float, WarmupCountdownStartTime)
	float WarmupCountdownStartTime;

	UPROPERTY(float, WarmupCountdownEndTime)
	float WarmupCountdownEndTime;

	AFortAthenaAircraft* GetAircraft(int AircraftIndex);

	int GetAliveTeams();

	void OnRep_GamePhase(EAthenaGamePhase OldGamePhase);
	void OnRep_CurrentPlaylistData();
	void OnRep_PlayersLeft();

	bool IsGameInitialized();

	UFortPlaylistAthena* GetCurrentPlaylist();
	void SetCurrentPlaylist(UFortPlaylistAthena* Playlist);

	FGameplayTagContainer GetAthenaPlaylistContextTags();

	TArray<TWeakObjectPtr<AFortPlayerStateAthena>>* FindTeamMembers(uint8 TeamIndex);
	TArray<TWeakObjectPtr<AFortPlayerStateAthena>>* FindSquadMembers(uint8 SquadId);

	bool IsResurrectionEnabled(AFortPlayerPawn* Pawn);
};