#include "ItemDefinitions/FortItem.h"
#include "Inventory/FortWorldItem.h"

#include "Curves/SimpleCurve.h"

#include "AttributeSet.h"

#include "Kismet/KismetSystemLibrary.h"

int UFortItemDefinition::GetMaxStackSize()
{
	if (UKismetSystemLibrary::GetEngineVersionNum() >= 4.25)
	{
		auto& ScalableFloat = *(FScalableFloat*)&MaxStackSizeInt;
		auto AthenaGameData = LoadObject<UCurveTable>(L"/Game/Athena/Balance/DataTables/AthenaGameData.AthenaGameData");

		for (auto&& Pair : AthenaGameData->GetAs<FSimpleCurve>())
		{
			if (Pair.Key == ScalableFloat.Curve.RowName)
				return Pair.Value->Keys[0].Value;
		}

		return ScalableFloat.Value;
	}

	return MaxStackSizeInt;
}

UFortItem* UFortItemDefinition::Create(int32 Count, int32 Level)
{
	struct
	{
		int32 Count;
		int32 Level;
		UFortWorldItem* ReturnValue;
	} params(Count, Level);

	this->ProcessEvent(L"CreateTemporaryItemInstanceBP", &params);

	return params.ReturnValue;
}

FGuid UFortItem::GetItemGuid()
{
	FGuid ReturnValue;
	this->ProcessEvent(L"GetItemGuid", &ReturnValue);

	return ReturnValue;
}

bool UFortItem::CanBeDropped()
{
	bool ReturnValue;
	this->ProcessEvent(L"CanBeDropped", &ReturnValue);

	return ReturnValue;
}