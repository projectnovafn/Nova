#include "Building/BuildingStructuralSupportSystem.h"

EFortStructuralGridQueryResults UBuildingStructuralSupportSystem::CanAddBuildingActorClassToGrid(UObject* WorldContextObject, UClass* BuildingClass, FVector Location, FRotator Rotation, bool bMirrored, TArray<ABuildingActor*>& ExistingBuilds)
{
    uint8_t Data[8]{};
    static auto CanAddBuildingActorClassToGrid = UKismetMemoryLibrary::Get<EFortStructuralGridQueryResults (*)(UObject*, UObject*, FVector&, FRotator&, bool, TArray<ABuildingActor*>&, void*)>(L"UBuildingStructuralSupportSystem::CanAddBuildingActorClassToGrid");

    return CanAddBuildingActorClassToGrid(WorldContextObject, BuildingClass, Location, Rotation, bMirrored, ExistingBuilds, Data);
}