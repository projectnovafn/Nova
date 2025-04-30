#include "Engine/ReplicationDriver.h"

int32 UReplicationDriver::ServerReplicateActors(float DeltaSeconds)
{
    static auto ServerReplicateActors = UKismetMemoryLibrary::Get<int32(*)(UReplicationDriver*, float)>(L"UFortReplicationGraph::ServerReplicateActors");

    return ServerReplicateActors(this, DeltaSeconds);
}