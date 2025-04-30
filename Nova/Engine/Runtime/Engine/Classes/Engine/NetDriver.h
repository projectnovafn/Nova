#pragma once
#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "UObject/UObjectGlobals.h"
#include "UObject/Object.h"

#include "NetConnection.h"
#include "World.h"

#include "ReplicationDriver.h"

class UNetDriver : public UObject
{
	GENERATED_UCLASS_BODY(UNetDriver, Engine)
public:

	UPROPERTY(UWorld*, World)
	UWorld* World;

	UPROPERTY(FName, NetDriverName)
	FName NetDriverName;

	UPROPERTY(UReplicationDriver*, ReplicationDriver)
	UReplicationDriver* ReplicationDriver;

	UPROPERTY(UNetConnection*, ServerConnection)
	UNetConnection* ServerConnection;

	UPROPERTY(TArray<UNetConnection*>, ClientConnections)
	TArray<UNetConnection*> ClientConnections;

	bool InitListen(FNetworkNotify* InNotify, FURL& ListenURL, bool bReuseAddressAndPort, FString& Error);
	void SetWorld(UWorld* InWorld);

	void TickFlush(float DeltaSeconds);
	int32 ServerReplicateActors(float DeltaSeconds);

	bool IsServer();
};