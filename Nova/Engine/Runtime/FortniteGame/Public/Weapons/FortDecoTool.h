#pragma once

#include "FortWeapon.h"

enum class EBuildingAttachmentType : uint8
{
	ATTACH_Floor = 0,
	ATTACH_Wall = 1,
	ATTACH_Ceiling = 2,
	ATTACH_Corner = 3,
	ATTACH_All = 4,
	ATTACH_WallThenFloor = 5,
	ATTACH_FloorAndStairs = 6,
	ATTACH_CeilingAndStairs = 7,
	ATTACH_None = 8,
	ATTACH_MAX = 9
};

class ABuildingSMActor;

class AFortDecoTool : public AFortWeapon
{
	GENERATED_UCLASS_BODY(AFortDecoTool, FortniteGame)

public:

	UPROPERTY(UFortItemDefinition*, ItemDefinition)
	UFortItemDefinition* ItemDefinition;

public:

	void OnRep_ItemDefinition();

public:

	void CallServerSpawnDeco(FVector Location, FRotator Rotation, ABuildingSMActor* AttachedActor, EBuildingAttachmentType InBuildingAttachmentType);
	void ServerSpawnDeco(FVector Location, FRotator Rotation, ABuildingSMActor* AttachedActor, EBuildingAttachmentType InBuildingAttachmentType);
	void ServerCreateBuildingAndSpawnDeco(FVector BuildingLocation, FRotator BuildingRotation, FVector Location, FRotator Rotation, EBuildingAttachmentType InBuildingAttachmentType);
};