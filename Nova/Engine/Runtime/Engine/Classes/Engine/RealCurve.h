#pragma once

#include "CoreMinimal.h"
#include "CoreUObject.h"

enum ERichCurveInterpMode : int
{
	RCIM_Linear,
	RCIM_Constant,
	RCIM_Cubic,
	RCIM_None
};
enum ERichCurveExtrapolation : int
{
	RCCE_Cycle,
	RCCE_CycleWithOffset,
	RCCE_Oscillate,
	RCCE_Linear,
	RCCE_Constant,
	RCCE_None
};

struct FCurveAttributes
{
	FCurveAttributes()
	{
		bHasPreExtrapolation = 0;
		bHasPostExtrapolation = 0;
		PreExtrapolation = ERichCurveExtrapolation::RCCE_None;
		PostExtrapolation = ERichCurveExtrapolation::RCCE_None;
	}

	bool HasPreExtrapolation() const { return bHasPreExtrapolation; }
	bool HasPostExtrapolation() const { return bHasPostExtrapolation; }

	ERichCurveExtrapolation GetPreExtrapolation() const { check(bHasPreExtrapolation);  return PreExtrapolation; }
	ERichCurveExtrapolation GetPostExtrapolation() const { check(bHasPostExtrapolation); return PostExtrapolation; }

	FCurveAttributes& SetPreExtrapolation(ERichCurveExtrapolation InPreExtrapolation) { bHasPreExtrapolation = 1;  PreExtrapolation = InPreExtrapolation;   return *this; }
	FCurveAttributes& SetPostExtrapolation(ERichCurveExtrapolation InPostExtrapolation) { bHasPostExtrapolation = 1; PostExtrapolation = InPostExtrapolation; return *this; }

	void UnsetPreExtrapolation() { bHasPreExtrapolation = 0; }
	void UnsetPostExtrapolation() { bHasPostExtrapolation = 0; }

	static FCurveAttributes MaskCommon(const FCurveAttributes& A, const FCurveAttributes& B)
	{
		FCurveAttributes NewAttributes;

		if (A.bHasPreExtrapolation && B.bHasPreExtrapolation && A.PreExtrapolation == B.PreExtrapolation)
		{
			NewAttributes.SetPreExtrapolation(A.PreExtrapolation);
		}

		if (A.bHasPostExtrapolation && B.bHasPostExtrapolation && A.PostExtrapolation == B.PostExtrapolation)
		{
			NewAttributes.SetPostExtrapolation(A.PostExtrapolation);
		}

		return NewAttributes;
	}

private:

	uint8 bHasPreExtrapolation : 1;
	uint8 bHasPostExtrapolation : 1;

	ERichCurveExtrapolation PreExtrapolation;
	ERichCurveExtrapolation PostExtrapolation;
};

struct FRealCurve
{
	GENERATED_USTRUCT_BODY(FRealCurve, Engine)

public:

	void SetDefaultValue(float InDefaultValue) { DefaultValue = InDefaultValue; }

	float GetDefaultValue() { return DefaultValue; }

	void ClearDefaultValue() { DefaultValue = MAX_flt; }

public:

	UPROPERTY_STRUCT(float, DefaultValue)
	float DefaultValue;

	UPROPERTY_STRUCT(TEnumAsByte<ERichCurveExtrapolation>, PreInfinityExtrap)
	TEnumAsByte<ERichCurveExtrapolation> PreInfinityExtrap;

	UPROPERTY_STRUCT(TEnumAsByte<ERichCurveExtrapolation>, PostInfinityExtrap)
	TEnumAsByte<ERichCurveExtrapolation> PostInfinityExtrap;
};