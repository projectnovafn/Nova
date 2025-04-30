#pragma once

#include "CoreUObject.h"

class UKismetMathLibrary : public UObject
{
	GENERATED_UCLASS_BODY(UKismetMathLibrary, Engine)

public:

	static float RandomFloatInRange(float Min, float Max);
	static int RandomIntegerInRange(int Min, int Max);

	static FRotator MakeRotFromX(FVector X);

	static FVector InverseTransformDirection(FTransform T, FVector Direction);
	static FVector InverseTransformLocation(FTransform T, FVector Location);

	static FRotator Quat_Rotator(FQuat Q);
};