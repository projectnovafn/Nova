#pragma once
#include "Engine.h"

class UFortMissionLibrary : public UObject
{
    GENERATED_UCLASS_BODY(UFortKismetLibrary, FortniteGame)

public:
    static void TeleportPlayerPawn(class AFortPlayerPawn* PlayerPawn, const struct FVector& DestLocation, const struct FRotator& DestRotation, bool bIgnoreCollision, bool bIgnoreSupplementalKillVolumeSweep);
};