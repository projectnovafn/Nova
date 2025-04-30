#pragma once
#include "UnrealMathSSE.h"

/** Vector that represents (1/255,1/255,1/255,1/255) */
extern const VectorRegister VECTOR_INV_255;

/**
* Below this weight threshold, animations won't be blended in.
*/
#define ZERO_ANIMWEIGHT_THRESH (0.00001f)

namespace GlobalVectorConstants
{
    static const VectorRegister AnimWeightThreshold = MakeVectorRegister(ZERO_ANIMWEIGHT_THRESH, ZERO_ANIMWEIGHT_THRESH, ZERO_ANIMWEIGHT_THRESH, ZERO_ANIMWEIGHT_THRESH);
    static const VectorRegister RotationSignificantThreshold = MakeVectorRegister(1.0f - DELTA * DELTA, 1.0f - DELTA * DELTA, 1.0f - DELTA * DELTA, 1.0f - DELTA * DELTA);
}