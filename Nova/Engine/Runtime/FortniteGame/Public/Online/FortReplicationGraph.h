#pragma once
#include "ReplicationGraph.h"

class AFortPlayerStateAthena;

class UFortReplicationGraph : public UReplicationGraph
{
	GENERATED_UCLASS_BODY(UFortReplicationGraph, FortniteGame)
public:

	void OnPlayerStateSquadAssign(AFortPlayerStateAthena* PS);
};