#include "Athena/AI/FortServerBotManagerAthena.h"

APawn* UFortServerBotManagerAthena::SpawnAI(FVector InSpawnLocation, FRotator InSpawnRotation, UObject* InBotData)
{
    struct
    {
        FVector InSpawnLocation;
        FRotator InSpawnRotation;
        UObject* InBotData;
        APawn* ReturnValue;
    } params(InSpawnLocation, InSpawnRotation, InBotData);

    this->ProcessEvent(L"SpawnAI", &params);

    return params.ReturnValue;
}