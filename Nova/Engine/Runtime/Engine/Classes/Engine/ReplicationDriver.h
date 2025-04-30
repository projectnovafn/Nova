#pragma once
#include "CoreMinimal.h"
#include "CoreUObject.h"

class UReplicationDriver : public UObject
{
	GENERATED_UCLASS_BODY(UReplicationDriver, Engine)
public:

	int32 ServerReplicateActors(float DeltaSeconds);
};