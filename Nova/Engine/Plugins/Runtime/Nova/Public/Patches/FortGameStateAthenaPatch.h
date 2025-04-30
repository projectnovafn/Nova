#pragma once
#include "Athena\FortGameStateAthena.h"

class AFortPlayerStateAthena;

class AFortGameStateAthenaPatch
{
private:
	static bool IsResurrectionEnabledHook(AFortGameStateAthena*, AFortPlayerPawn*);

	static TArray<TWeakObjectPtr<AFortPlayerStateAthena>>* FindTeamMembersHook(AFortGameStateAthena*, uint8);
	static TArray<TWeakObjectPtr<AFortPlayerStateAthena>>* FindSquadMembersHook(AFortGameStateAthena*, uint8);

public:

	static void Init();
};