#include "Engine/NetDriver.h"

bool UNetDriver::InitListen(FNetworkNotify* InNotify, FURL& ListenURL, bool bReuseAddressAndPort, FString& Error)
{
	static auto InitListen = UKismetMemoryLibrary::Get<bool (*)(UNetDriver*, FNetworkNotify*, FURL&, bool, FString&)>(L"UNetDriver::InitListen");

	return InitListen(this, InNotify, ListenURL, bReuseAddressAndPort, Error);
}

void UNetDriver::SetWorld(UWorld* InWorld)
{
	static auto SetWorld = UKismetMemoryLibrary::Get<void (*)(UNetDriver*, UWorld*)>(L"UNetDriver::SetWorld");

	return SetWorld(this, InWorld);
}

bool UNetDriver::IsServer()
{
	return ServerConnection == NULL;
}

void UNetDriver::TickFlush(float DeltaSeconds)
{
	if (IsServer() && ClientConnections.Num() > 0 && ClientConnections[0]->InternalAck == false)
	{
		ServerReplicateActors(DeltaSeconds);
	}
}
	
int32 UNetDriver::ServerReplicateActors(float DeltaSeconds)
{
	if (ReplicationDriver)
	{
		return ReplicationDriver->ServerReplicateActors(DeltaSeconds);
	}

	return 0;
}