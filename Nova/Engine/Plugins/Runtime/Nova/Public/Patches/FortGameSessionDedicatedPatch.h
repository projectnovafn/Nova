#pragma once
#include "Online/FortGameSessionDedicated.h"

class AFortGameSessionDedicatedPatch
{
private:

	static void HandleMatchAssignmentHook(AActor*, FMatchmakingDedicatedServerMatchAssignment&);
	static void RestartHook(AFortGameSessionDedicated*);

public:

	static void Init();
};