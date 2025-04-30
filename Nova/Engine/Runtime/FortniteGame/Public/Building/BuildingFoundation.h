#pragma once
#include "BuildingSMActor.h"

enum class EDynamicFoundationType : uint8
{
	Static = 0,
	StartEnabled_Stationary = 1,
	StartEnabled_Dynamic = 2,
	StartDisabled = 3
};

class FDynamicBuildingFoundationRepData
{
	GENERATED_USTRUCT_BODY(FDynamicBuildingFoundationRepData, FortniteGame)

public:

	UPROPERTY_STRUCT(uint8, EnabledState)
	uint8 EnabledState;
};

class ABuildingFoundation : public ABuildingSMActor
{
	GENERATED_UCLASS_BODY(ABuildingFoundation, FortniteGame)
public:

	UPROPERTY(EDynamicFoundationType, DynamicFoundationType)
	EDynamicFoundationType DynamicFoundationType;

	UPROPERTY(uint8, FoundationEnabledState)
	uint8 FoundationEnabledState;

	UPROPERTY(FDynamicBuildingFoundationRepData, DynamicFoundationRepData)
	FDynamicBuildingFoundationRepData DynamicFoundationRepData;

	UPROPERTY_BOOL(bServerStreamedInLevel)
	bool bServerStreamedInLevel;

	void OnRep_ServerStreamedInLevel();
	void OnRep_DynamicFoundationRepData();

	bool IsShown();
	void Show();
};