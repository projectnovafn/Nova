#pragma once

#include "Engine.h"

class ABuildingActor;

enum EFortStructuralGridQueryResults : uint8
{
    CanAdd = 0,
    ExistingActor = 1,
    Obstructed = 2,
    NoStructuralSupport = 3,
    InvalidActor = 4,
    ReachedLimit = 5,
    NoEditPermission = 6,
    PatternNotPermittedByLayoutRequirement = 7,
    ResourceTypeNotPermittedByLayoutRequirement = 8,
    BuildingAtRequirementsDisabled = 9,
    BuildingOtherThanRequirementsDisabled = 10
};

class UBuildingStructuralSupportSystem : public UObject
{
	GENERATED_UCLASS_BODY(UBuildingStructuralSupportSystem, FortniteGame)

public:

	static EFortStructuralGridQueryResults CanAddBuildingActorClassToGrid(UObject* WorldContextObject, UClass* BuildingClass, FVector Location, FRotator Rotation, bool bMirrored, TArray<ABuildingActor*>& ExistingBuilds);
};