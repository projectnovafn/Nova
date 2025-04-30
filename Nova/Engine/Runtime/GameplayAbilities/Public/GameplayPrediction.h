#pragma once
#include "Engine.h"
#include "Engine/NetSerialization.h"

class FPredictionKey
{
	GENERATED_USTRUCT_BODY(FPredictionKey, GameplayAbilities)

	unsigned char Pad[0x18];

public:
	UPROPERTY_STRUCT(int16, Current)
	int16 Current;

	UPROPERTY_STRUCT(int16, Base)
	int16 Base;
};