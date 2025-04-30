#pragma once
#include "Online/FortGameSessionDedicated.h"

class AFortGameSessionDedicatedAthena : public AFortGameSessionDedicated
{
	GENERATED_UCLASS_BODY(AFortGameSessionDedicatedAthena, FortniteGame)
public:

	int32 GetNumberOfPlayersInSquad(uint8 SquadId);
};