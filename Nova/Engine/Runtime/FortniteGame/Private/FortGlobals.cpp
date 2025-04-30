#include "FortGlobals.h"

FString UFortGlobals::GetBRMapName()
{
    if (GetClassProperty(L"BRMapFullName") != NULL)
        return L"/Game/Athena/Apollo/Maps/Apollo_Terrain.Apollo_Terrain";

    return L"/Game/Athena/Maps/Athena_Terrain.Athena_Terrain";
}

FString UFortGlobals::GetBRMapShortName()
{
    if (GetClassProperty(L"BRMapFullName") != NULL)
        return L"Apollo_Terrain";

    return L"Athena_Terrain";
}