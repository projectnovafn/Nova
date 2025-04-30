#include "Patches/FortGameSessionDedicatedAthenaPatch.h"
#include "KismetDetoursLibrary.h"

int32 (*_GetNumberOfPlayersInSquad)(AFortGameSessionDedicatedAthena*, uint8);

UDetour* GetNumberOfPlayersInSquadDetour = NULL;

int32 AFortGameSessionDedicatedAthenaPatch::GetNumberOfPlayersInSquadHook(AFortGameSessionDedicatedAthena* GameSession, uint8 SquadId)
{
    return GameSession->GetNumberOfPlayersInSquad(SquadId);
}

void AFortGameSessionDedicatedAthenaPatch::Init()
{
    _GetNumberOfPlayersInSquad = UKismetMemoryLibrary::Get<decltype(_GetNumberOfPlayersInSquad)>(L"AFortGameSessionDedicatedAthena::GetNumberOfPlayersInSquad");

    GetNumberOfPlayersInSquadDetour = new UDetour();
    GetNumberOfPlayersInSquadDetour->Init(_GetNumberOfPlayersInSquad, GetNumberOfPlayersInSquadHook);
    GetNumberOfPlayersInSquadDetour->Commit();
}