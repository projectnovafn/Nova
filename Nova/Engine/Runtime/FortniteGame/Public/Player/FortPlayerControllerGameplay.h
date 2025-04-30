#pragma once
#include "FortPlayerController.h"

class FGhostModeRepData
{
	GENERATED_USTRUCT_BODY(FGhostModeRepData, FortniteGame)
public:

	UPROPERTY_STRUCT(bool, bInGhostMode)
	bool bInGhostMode;

	UPROPERTY_STRUCT(UFortWorldItemDefinition*, GhostModeItemDef)
	UFortWorldItemDefinition* GhostModeItemDef;

	UPROPERTY_STRUCT(int, PreviousFocusedSlot)
	int PreviousFocusedSlot;

	UPROPERTY_STRUCT(float, TimeExitedGhostMode)
	float TimeExitedGhostMode;
};

class AFortPlayerControllerGameplay : public AFortPlayerController
{
	GENERATED_UCLASS_BODY(AFortPlayerControllerGameplay, FortniteGame)
public:

	UPROPERTY(TSoftObjectPtr<UFortItemDefinition>, CreativeMoveToolItemDefinition)
	TSoftObjectPtr<UFortItemDefinition> CreativeMoveToolItemDefinition;

	UPROPERTY(FGhostModeRepData, GhostModeRepData)
	FGhostModeRepData GhostModeRepData;

	UPROPERTY_NOTIFY(int32, FlyingModifierIndex)
	int32 FlyingModifierIndex;

	UPROPERTY_BOOL(bIsFlightSprinting)
	bool bIsFlightSprinting;

	void OnRep_IsFlightSprinting();

	void ServerCreativeSetFlightSpeedIndex(int32 Index);
	void ServerCreativeSetFlightSprint(bool bSprint);

	void StartGhostMode(UFortWorldItemDefinition* ItemProvidingGhostMode);
	void EndGhostMode();

	void ServerCreativeToggleFly();
};