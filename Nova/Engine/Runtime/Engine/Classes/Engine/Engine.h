#pragma once

#include "CoreMinimal.h"
#include "CoreUObject.h"

#include "KismetMemoryLibrary.h"

class UNetDriver;
class UWorld;

class UEngine : public UObject
{
	GENERATED_UCLASS_BODY(UEngine, Engine)

public:

	bool CreateNamedNetDriver(UWorld* InWorld, FName NetDriverName, FName NetDriverDefinition);
	UNetDriver* FindNamedNetDriver(UWorld* InWorld, FName NetDriverName);
};

inline UEngine* GEngine;