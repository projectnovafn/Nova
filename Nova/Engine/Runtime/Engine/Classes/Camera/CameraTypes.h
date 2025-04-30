#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "Engine/EngineTypes.h"

class FMinimalViewInfo
{
	GENERATED_USTRUCT_BODY(FMinimalViewInfo, Engine)

public:

	UPROPERTY_STRUCT(FVector, Location)
	FVector Location;

	UPROPERTY_STRUCT(FRotator, Rotation)
	FRotator Rotation;
};