#pragma once
#include "Athena/FortGameModeAthena.h"

class AFortGameModeAthenaPatch
{
private:

	static bool ReadyToStartMatchHook(AFortGameModeAthena*);
	static bool ReadyToEndMatchHook(AFortGameModeAthena*);
	static APawn* SpawnDefaultPawnForHook(AFortGameModeAthena*, AController*, AActor*);
	static UClass** GetGameSessionClassHook(AFortGameModeAthena*, UClass**);
	static void OnSafeZonePhaseChangedHook(AFortGameModeAthena*, int);
	static bool PlayerCanRestartHook(AFortGameModeAthena*, APlayerController*);
	static bool GenerateVoiceChatTokenHook(AFortGameModeAthena*, AFortPlayerControllerZone*, EVoiceChannelType, FString const&, FString&, FString&);

public:

	static void Init();
};