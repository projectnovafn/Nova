#include "Engine\Engine.h"

bool UEngine::CreateNamedNetDriver(UWorld* InWorld, FName NetDriverName, FName NetDriverDefinition)
{
    static auto CreateNamedNetDriver = UKismetMemoryLibrary::Get<bool (*)(UEngine*, UWorld*, FName, FName)>(L"UEngine::CreateNamedNetDriver");

    return CreateNamedNetDriver(this, InWorld, NetDriverName, NetDriverDefinition);
}

UNetDriver* UEngine::FindNamedNetDriver(UWorld* InWorld, FName NetDriverName)
{
    static auto FindNamedNetDriver = UKismetMemoryLibrary::Get<UNetDriver*(*)(UEngine*, UWorld*, FName)>(L"UEngine::FindNamedNetDriver");

    return FindNamedNetDriver(this, InWorld, NetDriverName);
}