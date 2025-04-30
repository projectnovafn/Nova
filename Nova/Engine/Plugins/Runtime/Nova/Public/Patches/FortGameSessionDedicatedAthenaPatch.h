#pragma once
#include "Athena/Online/FortGameSessionDedicatedAthena.h"

class AFortGameSessionDedicatedAthenaPatch
{
private:
	static int32 GetNumberOfPlayersInSquadHook(AFortGameSessionDedicatedAthena*, uint8);

public:

	static void Init();
};