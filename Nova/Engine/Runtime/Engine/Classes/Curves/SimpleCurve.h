#pragma once
#include "CoreUObject.h"

struct FSimpleCurveKey
{
	GENERATED_USTRUCT_BODY(FSimpleCurveKey, Engine)

public:

	float Time;
	float Value;
};

class FSimpleCurve
{
	GENERATED_USTRUCT_BODY(FSimpleCurve, Engine)

public:

	UPROPERTY_STRUCT(TArray<FSimpleCurveKey>, Keys)
	TArray<FSimpleCurveKey> Keys;
};