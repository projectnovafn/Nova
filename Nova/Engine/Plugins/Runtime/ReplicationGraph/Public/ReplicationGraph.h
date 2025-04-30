#pragma once
#include "Engine/ReplicationDriver.h"

class UNetDriver;

class UReplicationGraph : public UReplicationDriver
{
	GENERATED_UCLASS_BODY(UReplicationGraph, ReplicationGraph)
public:

	UPROPERTY(UNetDriver*, NetDriver)
	UNetDriver* NetDriver;
};