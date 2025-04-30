#pragma once
#include "FortGameStateBase.h"
#include "FortTeamInfo.h"

#include "Player/FortPlayerController.h"

class AFortGameState : public AFortGameStateBase
{
	GENERATED_UCLASS_BODY(AFortGameState, FortniteGame)
public:

	UPROPERTY_NOTIFY(TArray<FName>, AdditionalPlaylistLevelsStreamed)
	TArray<FName> AdditionalPlaylistLevelsStreamed;

	UPROPERTY(TArray<AFortTeamInfo*>, Teams)
	TArray<AFortTeamInfo*> Teams;

	AFortPlayerController* FindPlayerByProfile(UMcpProfileGroup* McpProfileGroup);
	AFortPlayerState* FindPlayerById(FUniqueNetIdRepl& UniqueId);
};