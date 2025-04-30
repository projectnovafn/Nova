#include "Online/FortReplicationGraph.h"
#include "Athena/Player/FortPlayerStateAthena.h"

void UFortReplicationGraph::OnPlayerStateSquadAssign(AFortPlayerStateAthena* PS)
{
	static auto OnPlayerStateSquadAssign = UKismetMemoryLibrary::Get<void (*)(UFortReplicationGraph*, AFortPlayerStateAthena*)>(L"UFortReplicationGraph::OnPlayerStateSquadAssign");

	OnPlayerStateSquadAssign(this, PS);
}