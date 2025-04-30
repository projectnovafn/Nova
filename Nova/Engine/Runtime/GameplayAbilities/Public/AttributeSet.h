#pragma once
#include "CoreUObject.h"
#include "Engine/CurveTable.h"

class FScalableFloat
{
public:
	float Value;
private:
	unsigned char Pad[0x4];
public:
	FCurveTableRowHandle Curve;
private:
	unsigned char Pad2[0x8];
};