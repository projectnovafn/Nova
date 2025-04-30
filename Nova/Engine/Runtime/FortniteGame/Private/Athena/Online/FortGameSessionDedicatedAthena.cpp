#include "Athena/Online/FortGameSessionDedicatedAthena.h"
#include "Athena/FortGameStateAthena.h"
#include "Athena/Player/FortPlayerStateAthena.h"

int32 AFortGameSessionDedicatedAthena::GetNumberOfPlayersInSquad(uint8 SquadId)
{
	auto GameState = Cast<AFortGameStateAthena>(GetWorld()->GameState);

	if (!GameState)
		return 0;

	int32 Players = 0;

	for (auto& Player : GameState->PlayerArray)
	{
		auto PlayerState = Cast<AFortPlayerStateAthena>(Player);

		if (!PlayerState)
			continue;

		if (PlayerState->SquadId == SquadId)
			Players++;
	}

	return Players;
}