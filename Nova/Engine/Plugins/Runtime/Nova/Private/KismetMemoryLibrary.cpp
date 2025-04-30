#pragma once
#include "KismetMemoryLibrary.h"

#include "UObject/Class.h"
#include "Vehicles/FortOctopusVehicle.h"
#include "Online/FortGameSessionDedicated.h"

#include <map>
#include <functional>
#include <fstream>

#include "vendor\memcury.h"

static std::map<std::wstring, std::function<void* ()>>& GetFunctionMap()
{
    static std::map<std::wstring, std::function<void* ()>> FunctionMap;
    return FunctionMap;
}

static std::map<std::wstring, std::function<uint32()>>& GetOffsetFunctionMap()
{
    static std::map<std::wstring, std::function<uint32()>> OffsetFunctionMap;
    return OffsetFunctionMap;
}

void* UKismetMemoryLibrary::GetInternal(const TCHAR* Name)
{
    static std::map<std::wstring, void*> CachedValues;

    auto& FunctionMap = GetFunctionMap();
    auto it = CachedValues.find(Name);
    if (it != CachedValues.end())
    {
        return it->second;
    }

    auto funcIt = FunctionMap.find(Name);
    if (funcIt != FunctionMap.end())
    {
        void* result = funcIt->second();
        CachedValues[Name] = result;
        return result;
    }

    return NULL;
}

uint32 UKismetMemoryLibrary::GetOffsetInternal(const TCHAR* Name)
{
    static std::map<std::wstring, uint32> CachedOffsets;

    auto& FunctionMap = GetOffsetFunctionMap();
    auto it = CachedOffsets.find(Name);
    if (it != CachedOffsets.end())
    {
        return it->second;
    }

    auto funcIt = FunctionMap.find(Name);
    if (funcIt != FunctionMap.end())
    {
        uint32 result = funcIt->second();
        CachedOffsets[Name] = result;
        return result;
    }

    return -1;
}

void UKismetMemoryLibrary::AddInternal(const TCHAR* Name, FunctionType Function)
{
    GetFunctionMap()[Name] = Function;
}

void UKismetMemoryLibrary::AddOffsetInternal(const TCHAR* Name, OffsetFunctionType Function)
{
    GetOffsetFunctionMap()[Name] = Function;
}

void UKismetMemoryLibrary::LoadCache()
{
    for (const auto& Entry : GetFunctionMap())
        UE_LOG(LogTemp, Log, TEXT("%s: %p %p"), Entry.first.c_str(), UKismetMemoryLibrary::Get<void*>(Entry.first.c_str()), (void*)(UKismetMemoryLibrary::Get<uintptr_t>(Entry.first.c_str()) - (uintptr_t)GetModuleHandleW(NULL)));

    for (const auto& Entry : GetOffsetFunctionMap())
        UKismetMemoryLibrary::Get(Entry.first.c_str());
}

template <typename T>
static uint32 FindOffset(void* Object, T Target, int Start = 0)
{
    auto Ptr = (uint8*)Object;

    for (int i = Start; i < 0x2000; i++)
    {
        if (*(T*)(Ptr + i) == Target)
            return i;
    }

    return -1;
}

static double GetEngineVersion()
{
    auto GetEngineVersion = Memcury::Scanner::FindPattern("40 53 48 83 EC 20 48 8B D9 E8 ? ? ? ? 48 8B C8 41 B8 04 ? ? ? 48 8B D3")
        .GetAs<FString * (*)(FString*)>();

    FString Result;
    auto EngineVersion = GetEngineVersion(&Result)->ToString();

    return std::stod(EngineVersion.substr(0, 4));
}

DEFINE_MEMORY_VARIABLE(GetEngineVersion, "GetEngineVersion")
{
    return Memcury::Scanner::FindPattern("40 53 48 83 EC 20 48 8B D9 E8 ? ? ? ? 48 8B C8 41 B8 04 ? ? ? 48 8B D3")
        .GetAs<FString * (*)(FString*)>();
}

DEFINE_MEMORY_VARIABLE(FMemory_Realloc, "FMemory::Realloc")
{
    return Memcury::Scanner::FindStringRef(L"AbilitySystem.Debug.NextTarget", 0, false)
        .ScanFor({ 0xE8 })
        .RelativeOffset(1)
        .GetAs<void*>();
}

DEFINE_MEMORY_VARIABLE(FName_ToString, "FName::ToString")
{
    return Memcury::Scanner::FindStringRef(L"Destroying blueprint contexts for Local Player ' %s ' of Game Instance ' %s '")
        .ScanFor({ 0xE8 }, false, 1)
        .RelativeOffset(1)
        .GetAs<void*>();
}

DEFINE_MEMORY_VARIABLE(UObject_ProcessEvent, "UObject::ProcessEvent")
{
    if (auto Addr = Memcury::Scanner::FindPatterns({ "40 55 56 57 41 54 41 55 41 56 41 57 48 81 EC ? ? ? ? 48 8D 6C 24 ? 48 89 9D ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C5 48 89 85 ? ? ? ? 8B 41 0C 45 33 F6 3B 05", "40 55 56 57 41 54 41 55 41 56 41 57 48 81 EC ? ? ? ? 48 8D 6C 24 ? 48 89 9D ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C5 48 89 85 ? ? ? ? 4D 8B F8" }, false).GetAs<void*>())
        return Addr;

    if (auto Addr = Memcury::Scanner::FindStringRef(L"UMeshNetworkComponent::ProcessEvent: Invalid mesh network node type: %s", 0, false)
        .ScanFor({ 0x5F })
        .ScanFor({ 0xE9 })
        .RelativeOffset(1)
        .GetAs<void*>())
        return Addr;

    return Memcury::Scanner::FindStringRef(L"AccessNoneNoContext")
        .ScanFor({ 0x40, 0x55 })
        .GetAs<void*>();
}

DEFINE_MEMORY_VARIABLE(StaticFindObject, "StaticFindObject")
{
    return Memcury::Scanner::FindStringRef(L"Illegal call to StaticFindObject() while serializing object data!", 0, false)
        .ScanFor({ 0x48, 0x89, 0x5C }, false)
        .GetAs<void*>();
}

DEFINE_MEMORY_VARIABLE(StaticLoadObject, "StaticLoadObject")
{
    return Memcury::Scanner::FindStringRef(L"STAT_LoadObject", 0, false)
        .ScanFor({ 0x4C, 0x89 }, false)
        .GetAs<void*>();
}

DEFINE_MEMORY_VARIABLE(StaticDuplicateObject, "StaticDuplicateObject")
{
    return Memcury::Scanner::FindPattern("4C 89 44 24 ? 55 53 56 57 41 54 41 56 41 57 48 8D 6C 24 ? 48 81 EC ? ? ? ? 48 8B 05")
        .GetAs<void*>();
}

DEFINE_MEMORY_VARIABLE(FWindowsPlatformMisc_RequestExit, "FWindowsPlatformMisc::RequestExit")
{
    return Memcury::Scanner::FindPattern("40 53 48 83 EC ? 80 3D ? ? ? ? ? 0F B6 D9 72 ? 48 8B 05 ? ? ? ? 4C 8D 44 24 ? 48 89 44 24 ? 41 B9 ? ? ? ? 0F B6 C1 33 D2 89 44 24 ? 33 C9 48 8D 05 ? ? ? ? 48 89 44 24 ? E8 ? ? ? ? 48 8D 0D")
        .GetAs<void*>();
}

DEFINE_MEMORY_VARIABLE(FWindowsPlatformMisc_RequestExitWithStatus, "FWindowsPlatformMisc::RequestExitWithStatus")
{
    return Memcury::Scanner::FindStringRef(L"FPlatformMisc::RequestExitWithStatus(%i, %d) - return code will be ignored by the generic implementation.", 0, false)
        .ScanFor({ 0x40, 0x53 }, false)
        .GetAs<void*>();
}

DEFINE_MEMORY_VARIABLE(GIsServer, "GIsServer")
{
    if (GetEngineVersion() >= 4.25)
    {
        return Memcury::Scanner::FindStringRef(L"STAT_UpdateLevelStreaming", 0, false)
            .ScanFor({ 0x38, 0x1D }, false)
            .RelativeOffset(2)
            .GetAs<uint8*>();
    }

    return Memcury::Scanner::FindStringRef(L"STAT_UpdateLevelStreaming", 0, false)
        .ScanFor({ 0x80, 0x3D }, false, 1)
        .RelativeOffset(2)
        .GetAs<uint8*>() + 1;
}

DEFINE_MEMORY_VARIABLE(GIsClient, "GIsClient")
{
    if (GetEngineVersion() >= 4.25)
    {
        return Memcury::Scanner::FindStringRef(L"STAT_UpdateLevelStreaming", 0, false)
            .ScanFor({ 0x38, 0x1D }, false)
            .RelativeOffset(2)
            .GetAs<uint8*>() - 1;
    }

    return Memcury::Scanner::FindStringRef(L"STAT_UpdateLevelStreaming", 0, false)
        .ScanFor({ 0x80, 0x3D }, false, 1)
        .RelativeOffset(2)
        .GetAs<uint8*>();
}

DEFINE_MEMORY_VARIABLE(GEngine, "GEngine")
{
    return Memcury::Scanner::FindStringRef(L"STAT_FEngineLoop_TickFPSChart")
        .ScanFor({ 0x48, 0x8B })
        .RelativeOffset(3)
        .GetAs<void*>();
}

DEFINE_MEMORY_VARIABLE(FName_FName, "FName::FName")
{
    return Memcury::Scanner::FindStringRef(L"Unrecognized property %s on class %s")
        .ScanFor(GetEngineVersion() >= 4.26 ? std::vector<uint8>{ 0x74, 0x77 } : std::vector<uint8>{ 0x48, 0x8B, 0xCB }, false)
        .ScanFor({ 0xE8 }, false)
        .RelativeOffset(1)
        .GetAs<void*>();
}

DEFINE_MEMORY_VARIABLE(FilterCommandLine, "FilterCommandLine")
{
    auto addr = Memcury::Scanner::FindPattern("48 89 5C 24 ? 48 89 74 24 ? 48 89 7C 24 ? 4C 89 74 24 ? 55 48 8B EC 48 83 EC 60 45 33 F6 44 39 35");
    if (!addr.IsValid())
    {
        addr = Memcury::Scanner::FindPattern("66 89 ? ? ? 48 8B ? ? ? 48 83 C4 20 5F E9").RelativeOffset(16);
    }

    return addr.GetAs<void*>();
}

DEFINE_MEMORY_VARIABLE(AFortGameSessionDedicated_Restart, "AFortGameSessionDedicated::Restart")
{
    auto addr = Memcury::Scanner::FindPattern("40 53 55 56 57 41 56 48 83 EC ? 48 8B F9 E8 ? ? ? ? 33 ED");

    if (addr.IsValid())
        return addr.GetAs<void*>();

    return Memcury::Scanner::FindStringRef(L"AFortGameSessionDedicated::Restart: Starting to restart dedicated server")
        .ScanFor({ 0x41, 0x56 }, false)
        .ScanFor({ 0x5C }, false)
        .ScanFor({ 0x48 }, false)
        .GetAs<void*>();
}

DEFINE_MEMORY_VARIABLE(AFortGameStateAthena_FindSquadMembers, "AFortGameStateAthena::FindSquadMembers")
{ 
    return Memcury::Scanner::FindPattern("E8 ? ? ? ? 45 33 F6 45 8B FE")
        .RelativeOffset(1)
        .GetAs<void*>();
}

DEFINE_MEMORY_VARIABLE(AFortGameStateAthena_FindTeamMembers, "AFortGameStateAthena::FindTeamMembers")
{
    return Memcury::Scanner::FindPattern("E8 ? ? ? ? 48 8B 18 48 63 40 ? 48 8D 04 C3")
        .RelativeOffset(1)
        .GetAs<void*>();
}

DEFINE_MEMORY_VARIABLE(AFortPlayerControllerAthena_FindTeamMembers, "AFortPlayerControllerAthena::FindTeamMembers")
{
    return Memcury::Scanner::FindPattern("48 89 5C 24 ? 57 48 83 EC ? 48 8B D9 E8 ? ? ? ? 48 8B F8 48 85 C0 74 ? 48 8B 9B")
        .GetAs<void*>();
}

DEFINE_MEMORY_VARIABLE(UNetConnection_SetClientLoginState, "UNetConnection::SetClientLoginState")
{
    return Memcury::Scanner::FindStringRef(L"Join succeeded: %s")
        .ScanFor({ 0xBA, 0x03 })
        .ScanFor({ 0xE8 })
        .RelativeOffset(1)
        .GetAs<void*>();
}

DEFINE_MEMORY_VARIABLE(UNetConnection_Close, "UNetConnection::Close")
{
    return Memcury::Scanner::FindStringRef(L"UNetConnection::Close: %s, Channels: %i, Time: %s")
        .ScanFor({ 0x48, 0x8B, 0xC4 }, false)
        .GetAs<void*>();
}

DEFINE_MEMORY_VARIABLE(UUpdateManager_StartPatchCheck, "UUpdateManager::StartPatchCheck")
{
    if (auto Addr = Memcury::Scanner::FindPatterns({ "40 53 41 57 48 83 EC 38 48 8B 01 4C 8B F9", "40 55 53 41 54 41 55 48 8D 6C 24 ? 48 81 EC ? ? ? ? 33 DB 4C 8B E1 89 5D 6F E8 ? ? ? ? 48 8B D0 49 8B CC E8 ? ? ? ? 48 8B C8 C6 45 77 03 4C 8B E8 88 5D 6F E8", "40 55 53 57 41 56 41 57 48 8D ? ? ? 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 ? ? 48 8B 01 48 8B F9 FF 90" }).GetAs<void*>())
        return Addr;

    return Memcury::Scanner::FindStringRef(L"No local player to perform check!")
        .ScanFor({ 0x40, 0x55 }, false)
        .GetAs<void*>();
}

DEFINE_MEMORY_VARIABLE(UUpdateManager_PatchCheckComplete, "UUpdateManager::PatchCheckComplete")
{
    return Memcury::Scanner::FindPatterns({ "48 8B 01 88 91 ? ? ? ? 84 D2", "48 89 5C 24 ? 55 56 57 48 83 EC 40 0F B6 EA 48 8B F1 E8" })
        .GetAs<void*>();
}

DEFINE_MEMORY_VARIABLE(TournamentFailedConfigure, "TournamentFailedConfigure")
{
    return Memcury::Scanner::FindStringRef(L"MMS - successfully configured beacon, registering players", 0, false)
        .ScanFor({ 0x0F, 0x84 }, false)
        .GetAs<void*>();
}

DEFINE_MEMORY_VARIABLE(FMatchmakingServiceDedicatedServer_Finalize, "FMatchmakingServiceDedicatedServer::Finalize")
{
    return Memcury::Scanner::FindStringRef(L"STAT_FMatchmakingServiceDedicatedServer_Finalize_LambdaTicker")
        .ScanFor({ 0x40, 0x53 }, false)
        .GetAs<void*>();
}

DEFINE_MEMORY_VARIABLE(FMatchmakingServiceDedicatedServer_HandleWebSocketError, "FMatchmakingServiceDedicatedServer::HandleWebSocketError")
{
    return Memcury::Scanner::FindPattern("40 53 41 56 48 83 EC ? 80 79 ? ? 48 8B DA")
        .GetAs<void*>();
}

DEFINE_MEMORY_VARIABLE(DedicatedServerReady, "DedicatedServerReady")
{
    if (GetEngineVersion() >= 4.25)
    {
        return Memcury::Scanner::FindStringRef(L"Dedicated Server Ready!")
            .ScanFor({ 0x48, 0x85, 0xC0 }, false)
            .ScanFor({ 0x0F, 0x84 })
            .AbsoluteOffset(1)
            .GetAs<void*>();
    }

    return Memcury::Scanner::FindStringRef(L"Dedicated Server Ready!")
        .ScanFor({ 0x48, 0x8B, 0xD8 }, false)
        .ScanFor({ 0x0F, 0x84 })
        .AbsoluteOffset(1)
        .GetAs<void*>();
}

DEFINE_MEMORY_VARIABLE(FOnlineSubsystemMcp_ProcessRequestAsClient, "FOnlineSubsystemMcp::ProcessRequestAsClient")
{
    return Memcury::Scanner::FindStringRef(L"Queuing Mcp request for client=%s url=%s")
        .ScanFor({ 0x48, 0x89, 0x5C }, false)
        .GetAs<void*>();
}

DEFINE_MEMORY_VARIABLE(FOnlineSubsystemMcp_ProcessRequestAsUser, "FOnlineSubsystemMcp::ProcessRequestAsUser")
{
    return Memcury::Scanner::FindPattern("E8 ? ? ? ? 48 8B 5D 08 84 C0")
        .RelativeOffset(1)
        .GetAs<void*>();
}

DEFINE_MEMORY_VARIABLE(FServiceConfigMcp_GetServicePermissionsById, "FServiceConfigMcp::GetServicePermissionsById")
{
    return Memcury::Scanner::FindPattern("E8 ? ? ? ? 48 85 C0 74 72 48 8B 0F")
        .RelativeOffset(1)
        .GetAs<void*>();
}

DEFINE_MEMORY_VARIABLE(LoadEngineFonts, "LoadEngineFonts")
{
    return Memcury::Scanner::FindPatterns({ "48 89 5C 24 ? 56 48 83 EC 50 33 F6 48 89", "48 89 5C 24 ? 57 48 83 EC 50 33 FF 48 89 74 24 ? 48 39 3D", "40 53 48 83 EC ? 33 DB 48 89 ? ? ? 48 39 1D ? ? ? ? 48 89 BC ? ? 00 00 00" })
        .GetAs<void*>();
}

DEFINE_MEMORY_VARIABLE(CheckImageIntegrity, "CheckImageIntegrity")
{
    return Memcury::Scanner::FindPatterns({ "48 83 EC 28 48 8D 4C 24 ? FF 15 ? ? ? ? F2 0F 10 05", "48 83 EC 38 48 8D 4C 24 ? 0F 29 74 24" })
        .GetAs<void*>();
}

DEFINE_MEMORY_VARIABLE(IntegrityCheck, "IntegrityCheck")
{
    return Memcury::Scanner::FindPattern("48 83 EC 28 FF 51 08 B0 01 48 83 C4 28 C3")
        .GetAs<void*>();
}

DEFINE_MEMORY_VARIABLE(Broadcast, "Broadcast")
{
    if (GetEngineVersion() <= 4.21)
        return NULL;

    return Memcury::Scanner::FindPatterns({ "E8 ? ? ? ? 48 8D 8D ? ? ? ? E8 ? ? ? ? F3 0F 10 0D ? ? ? ? 48 8D 8D ? ? ? ? 4C 8B C0 E8 ? ? ? ? E8 ? ? ? ? 84 C0 0F 84", "E8 ? ? ? ? 48 8D 4D ? E8 ? ? ? ? F3 0F 10 0D ? ? ? ? 48 8D 8D ? ? ? ? 4C 8B C0 E8 ? ? ? ? 48 8B 5D ? 48 85 DB 74 2E 8B C7 F0 0F C1 43 ? 83 F8 ? 75 22 48 8B 03 48 8B CB FF 10 8B C7 F0 0F C1 43 ? 83 F8 ? 75 0E 48 8B 03 BA ? 00 00 00 48 8B CB FF 50 ? E8" }, false)
        .GetAs<void*>();
}

DEFINE_MEMORY_VARIABLE(CollectGarbageInternal, "CollectGarbageInternal")
{
    return Memcury::Scanner::FindStringRef(L"TryCollectGarbage: forcing GC after %d skipped attempts.")
        .ScanFor({ 0x41, 0x8B })
        .ScanFor({ 0xE8 })
        .RelativeOffset(1)
        .GetAs<void*>();
}

DEFINE_MEMORY_VARIABLE(CheckPawnOverlap, "CheckPawnOverlap")
{
    return Memcury::Scanner::FindPattern("E8 ? ? ? ? 44 0F B6 F0 84 C0 0F 84 ? ? ? ? 48 8D 97", false)
        .RelativeOffset(1)
        .GetAs<void*>();
}

DEFINE_MEMORY_VARIABLE(ReplicatedDataMap, "ReplicatedDataMap")
{
    if (GetEngineVersion() >= 4.22)
        return NULL;

    return Memcury::Scanner::FindStringRef(L"Ability %s is overriding pending replicated target data.")
        .ScanFor({ 0x48, 0x03 }, false)
        .ScanFor({ 0xE8 }, false)
        .RelativeOffset(1)
        .GetAs<void*>();
}

DEFINE_MEMORY_OFFSET(AGameMode_GetGameSessionClass, "AGameMode::GetGameSessionClass")
{
    return *Memcury::Scanner::FindPatterns({ "41 FF 90 ? ? ? ? 33 FF 48 8B D8", "41 FF 90 ? ? ? ? 48 8B F8 48 83 38 00", "? FF 90 ? ? ? ? 48 8B F8 48 39 18" })
        .AbsoluteOffset(3)
        .GetAs<uint32_t*>() / 8;
}

DEFINE_MEMORY_VARIABLE(AFortPickup_OnAboutToEnterBackpack, "AFortPickup::OnAboutToEnterBackpack")
{
    return Memcury::Scanner::FindPatterns({ "40 53 56 57 48 83 EC 30 4C 89 6C 24", "40 53 56 48 83 EC 38 4C 89 6C 24", "48 89 5C 24 ? 57 48 83 EC 20 48 8B D9 48 8B 89 ? ? ? ? 48 85 C9 74 20 48 8D", "4C 8B DC 53 55 56 48 83 EC 60", "48 83 EC 28 4C 8B D1 E8 ? ? ? ? 84 C0 75 07" })
        .GetAs<void*>();
}

DEFINE_MEMORY_VARIABLE(AFortGameModeAthena_AddToAlivePlayers, "AFortGameModeAthena::AddToAlivePlayers")
{
    return Memcury::Scanner::FindStringRef(L"FortGameModeAthena: Player [%s] doesn't have a valid PvP team, and won't be added to the alive players list.")
        .ScanFor({ 0x48, 0x85, 0xD2 }, false)
        .GetAs<void*>();
}

DEFINE_MEMORY_VARIABLE(AFortGameModeAthena_RemoveFromAlivePlayers, "AFortGameModeAthena::RemoveFromAlivePlayers")
{
    return Memcury::Scanner::FindStringRef(L"FortGameModeAthena: RemoveFromAlivePlayers called for Player [%s] who wasn't in the AlivePlayers list.")
        .ScanFor({ 0x4C, 0x89, 0x4C }, false)
        .GetAs<void*>();
}

DEFINE_MEMORY_VARIABLE(AFortGameModeAthena_StartMatchAbandonProcess, "AFortGameModeAthena::StartMatchAbandonProcess")
{
    return Memcury::Scanner::FindStringRef(L"[AFortGameModeAthena::StartMatchAbandonProcess] Starting match abandon process for %f seconds", 0, false)
        .ScanFor({ 0x40, 0x55 }, false)
        .GetAs<void*>();
}

DEFINE_MEMORY_VARIABLE(AFortGameModeAthena_OnSafeZonePhaseChanged, "AFortGameModeAthena::OnSafeZonePhaseChanged")
{
    return Memcury::Scanner::FindPatterns({ "E8 ? ? ? ? EB 42 80 BA", "E9 ? ? ? ? 48 8B C1 40 38 B9", "E8 ? ? ? ? EB 47 80 B9", "E8 ? ? ? ? EB 34 80 B9 ? ? ? ? ? 48 8B C1", "E8 ? ? ? ? EB 42 80 B9", "E8 ? ? ? ? EB 31 80 B9" }, false)
        .RelativeOffset(1)
        .GetAs<void*>();
}

DEFINE_MEMORY_VARIABLE(AFortGameModeZone_GenerateVoiceChatToken, "AFortGameModeZone::GenerateVoiceChatToken")
{
    return Memcury::Scanner::FindPatterns({ "4C 89 4C 24 ? 55 56 41 56 41 57", "40 55 53 56 57 48 8D 6C 24 ? 48 81 EC ? ? ? ? 80 B9 ? ? ? ? ? 49 8B F1" })
        .GetAs<void*>();
}

DEFINE_MEMORY_VARIABLE(UPlaysetLevelStreamComponent_LoadPlaysetAsync, "UPlaysetLevelStreamComponent::LoadPlaysetAsync")
{
    return Memcury::Scanner::FindStringRef(L"UPlaysetLevelStreamComponent::LoadPlayset Error: no owner for %s")
        .ScanFor({ 0x48, 0x89, 0x5C }, false)
        .GetAs<void*>();
}

DEFINE_MEMORY_VARIABLE(UPlaysetLevelStreamComponent_UnloadPlaysetAsync, "UPlaysetLevelStreamComponent::UnloadPlaysetAsync")
{
    return Memcury::Scanner::FindStringRef(L"UPlaysetLevelStreamComponent::UnloadPlayset %s")
        .ScanFor({ 0x40, 0x55 }, false)
        .GetAs<void*>();
}

DEFINE_MEMORY_VARIABLE(AFortPlayerControllerAthena_IsTeamDead, "AFortPlayerControllerAthena::IsTeamDead")
{
    return Memcury::Scanner::FindPattern("E8 ? ? ? ? 84 C0 0F 85 ? ? ? ? 48 8B BB ? ? ? ? 48 8D 05")
        .RelativeOffset(1)
        .GetAs<void*>();
}

DEFINE_MEMORY_VARIABLE(UWorld_Listen, "UWorld::Listen")
{
    return Memcury::Scanner::FindStringRef(L"LoadMap: failed to Listen(%s)")
        .ScanFor({ 0x84, 0xC0 }, false)
        .ScanFor({ 0xE8 }, false)
        .GetAs<void*>();
}

DEFINE_MEMORY_VARIABLE(UWorld_BeginPlay, "UWorld::BeginPlay")
{
    if (GetEngineVersion() >= 4.22)
        return NULL;

    return Memcury::Scanner::FindPattern("40 53 48 83 EC ? 48 8B D9 48 8B 89 ? ? ? ? 48 85 C9 74 ? 48 8B 01 FF 90 ? ? ? ? 48 8B 8B ? ? ? ? 48 85 C9 74 ? 48 8B 01 48 83 C4 ? 5B 48 FF A0", false)
        .GetAs<void*>();
}

DEFINE_MEMORY_VARIABLE(UWorld_Tick, "UWorld::Tick")
{
    return Memcury::Scanner::FindStringRef(L"STAT_WorldTickTime", 0, false)
        .ScanFor({ 0x48, 0x8B, 0xC4 }, false)
        .GetAs<void*>();
}

DEFINE_MEMORY_VARIABLE(UEngine_CreateNamedNetDriver, "UEngine::CreateNamedNetDriver")
{
    return Memcury::Scanner::FindStringRef(L"RecordReplay: failed to create demo net driver!")
        .ScanFor({ 0x84, 0xC0 }, false)
        .ScanFor({ 0xE8 }, false)
        .RelativeOffset(1)
        .GetAs<void*>();
}

DEFINE_MEMORY_VARIABLE(UEngine_FindNamedNetDriver, "UEngine::FindNamedNetDriver")
{
    if (GetEngineVersion() >= 4.25)
    {
        return Memcury::Scanner::FindStringRef(L"RecordReplay: failed to create demo net driver!")
            .ScanFor({ 0x48, 0x8B, 0x0D })
            .ScanFor({ 0xE8 })
            .RelativeOffset(1)
            .GetAs<void*>();
    }

    return Memcury::Scanner::FindStringRef(L"RecordReplay: failed to create demo net driver!")
        .ScanFor({ 0x4C })
        .ScanFor({ 0xE8 })
        .RelativeOffset(1)
        .GetAs<void*>();
}

DEFINE_MEMORY_VARIABLE(UNetDriver_InitListen, "UNetDriver::InitListen")
{
    return Memcury::Scanner::FindStringRef(L"Failed to init net driver ListenURL: %s: %s")
        .ScanFor({ 0x48, 0x89, 0x5C }, false)
        .GetAs<void*>();
}

DEFINE_MEMORY_VARIABLE(UNetDriver_SetWorld, "UNetDriver::SetWorld")
{
    if (GetEngineVersion() >= 4.26)
    {
        return Memcury::Scanner::FindPattern("40 55 56 41 56 48 8B EC 48 83 EC ? 48 89 5C ? ? 4C 8B F2 48 8B 99 ? ? 00 00")
            .GetAs<void*>();
    }

    return Memcury::Scanner::FindStringRef(L"AOnlineBeaconHost::InitHost failed")
        .ScanFor({ 0xE8 }, false, 1)
        .RelativeOffset(1)
        .GetAs<void*>();
}

DEFINE_MEMORY_VARIABLE(UWorld_FindCollectionByType, "UWorld::FindCollectionByType")
{
    return Memcury::Scanner::FindStringRef(L"LevelPrefixOverride=")
        .ScanFor(GetEngineVersion() == 4.21 ? std::vector<uint8_t>{ 0x33, 0xD2 } : std::vector<uint8_t>{ 0xB2, 0x01 })
        .ScanFor({ 0xE8 })
        .RelativeOffset(1)
        .GetAs<void*>();
}

DEFINE_MEMORY_VARIABLE(AFortPlayerController_RemoveItem, "AFortPlayerController::RemoveItem")
{
    return Memcury::Scanner::FindPatterns({ "48 83 EC 48 80 B9 ? ? ? ? ? 74 22", "48 83 EC 48 80 B9 ? ? ? ? ? 74 27", "48 83 EC 38 80 B9 ? ? ? ? ? 74 19", "48 83 EC 48 80 B9 ? ? ? ? ? 74 1A" })
        .ScanFor({ 0x3 })
        .GetAs<void*>();
}

DEFINE_MEMORY_VARIABLE(AFortPlayerControllerAthena_EnterAircraft, "AFortPlayerControllerAthena::EnterAircraft")
{
    return Memcury::Scanner::FindStringRef(L"EnterAircraft: [%s] is attempting to enter aircraft after having already exited.")
        .ScanFor({ 0x40, 0x55 }, false)
        .GetAs<void*>();
}

DEFINE_MEMORY_VARIABLE(AFortPlayerControllerAthena_ExitAircraft, "AFortPlayerControllerAthena::ExitAircraft")
{
    auto Addr = Memcury::Scanner::FindPattern("E8 ? ? ? ? 48 8B 5C 24 ? 48 83 C4 20 5F C3 90 E0 65", false).RelativeOffset(1);

    if (!Addr.IsValid())
        Addr = Memcury::Scanner::FindPattern("4C 8B DC 55 56 49 8D AB ? ? ? ? 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 85 ? ? ? ? 80 B9 ? ? ? ? ? 48 8B F1 0F 84 ? ? ? ? 49 89 5B 10", false);

    if (Addr.IsValid())
        return Addr.GetAs<void*>();;

    return Memcury::Scanner::FindStringRef(L"ExitAircraft: %s (Pawn %s)")
        .ScanFor(GetEngineVersion() <= 4.21 ? std::vector<uint8_t>{0x4C, 0x8B, 0xDC} : std::vector<uint8_t>{0x40, 0x55}, false)
        .GetAs<void*>();
}

DEFINE_MEMORY_VARIABLE(AFortPlayerPawnAthena_HandleZiplineStateChanged, "AFortPlayerPawnAthena::HandleZiplineStateChanged")
{
    auto StrRef = Memcury::Scanner::FindStringRef(L"ZIPLINES!! Role(%s)  AFortPlayerPawn::ServerEndZiplining_Implementation bFromJump=%d", 0, false);

    if (!StrRef.IsValid())
        StrRef = Memcury::Scanner::FindStringRef(L"ZIPLINES!! GetLocalRole()(%s)  AFortPlayerPawn::ServerEndZiplining_Implementation bFromJump=%d", 0, false);

    if (!StrRef.IsValid())
        return NULL;

    return StrRef
        .ScanFor({ 0x74, 0x05 })
        .ScanFor({ 0xE8 }, true, 1)
        .RelativeOffset(1)
        .GetAs<void*>();
} 

DEFINE_MEMORY_VARIABLE(AFortPlayerController_GetPlayerViewpoint, "AFortPlayerController::GetPlayerViewpoint")
{
    static auto DefaultPlayerController = FindObject<UObject>(L"/Script/FortniteGame.Default__FortPlayerControllerAthena");
    static auto SpawnFailedAddress = Memcury::Scanner::FindStringRef(L"%s failed to spawn a pawn")
        .ScanFor({ 0x40, 0x53 }, false)
        .GetAs<void*>();

    for (int i = 0; i < 0x200; i++)
    {
        if (DefaultPlayerController->VTable[i] == SpawnFailedAddress)
        {
            return DefaultPlayerController->VTable[i - 1];
        }
    }

    return NULL;
}

DEFINE_MEMORY_VARIABLE(AFortPlayerPawn_GetViewRotation, "AFortPlayerPawn::GetViewRotation")
{
    return Memcury::Scanner::FindPattern("48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 57 48 83 EC 30 48 83 B9 ? ? ? ? ? 48 8B EA")
        .GetAs<void*>();
}

DEFINE_MEMORY_VARIABLE(UFortGadgetItemDefinition_ApplyGadgetData, "UFortGadgetItemDefinition::ApplyGadgetData")
{
    auto ApplyInternalAddr = Memcury::Scanner::FindPattern("48 89 5C 24 ? 48 89 6C 24 ? 48 89 7C 24 ? 41 54 41 56 41 57 48 83 EC 20 41 0F").GetAs<void*>();
    auto ApplyInternalRef = Memcury::Scanner::FindPointerRef(ApplyInternalAddr);

    if (!ApplyInternalRef.IsValid())
        return ApplyInternalAddr;

    return ApplyInternalRef.ScanFor({ 0x48, 0x89 }, false)
        .GetAs<void*>();
}

DEFINE_MEMORY_VARIABLE(UObject_GetInterfaceAddress, "UObject::GetInterfaceAddress")
{
    return Memcury::Scanner::FindPattern("48 89 5C 24 ? 48 89 74 24 ? 57 48 83 EC 20 33 DB 48 8B FA 48 8B F1 48 85 D2 0F 84 ? ? ? ? 8B 82 ? ? ? ? C1 E8")
        .GetAs<void*>();
}

DEFINE_MEMORY_VARIABLE(UObject_IsValidLowLevelFast, "UObject::IsValidLowLevelFast")
{
    if (GetEngineVersion() <= 4.21)
    {
        return Memcury::Scanner::FindStringRef(L"'this' pointer is misaligned.")
            .ScanFor({ 0x40, 0x53 }, false)
            .GetAs<void*>();
    }

    return Memcury::Scanner::FindStringRef(L"'this' pointer is invalid.")
        .ScanFor({ 0x57 }, false)
        .ScanFor({ 0x48, 0x89 }, false)
        .GetAs<void*>();
}

DEFINE_MEMORY_VARIABLE(AActor_ReceiveActorBeginOverlap, "AActor::ReceiveActorBeginOverlap")
{
    return Memcury::Scanner::FindPattern("E8 ? ? ? ? 80 BF ? ? ? ? ? 0F 85 ? ? ? ? 48 85 DB 0F 84 ? ? ? ? 48 8B CB E8 ? ? ? ? 84 C0 0F 85")
        .RelativeOffset(1)
        .GetAs<void*>();
}

DEFINE_MEMORY_VARIABLE(UFortUIStateWidget_Login_HandleLoginRequestComplete, "UFortUIStateWidget_Login::HandleLoginRequestComplete")
{
    return Memcury::Scanner::FindStringRef(L"[UFortUIStateWidget_Login::HandleLoginRequestComplete] [Result=%d] [ErrorReason=%s]", 1)
        .ScanFor({ 0x40, 0x55 }, false)
        .GetAs<void*>();
}

DEFINE_MEMORY_VARIABLE(ChangeGameSessionId, "ChangeGameSessionId")
{
    return Memcury::Scanner::FindStringRef(L"Changing GameSessionId from '%s' to '%s'")
        .ScanFor({ 0x40, 0x55 }, false)
        .GetAs<void*>();
}

DEFINE_MEMORY_VARIABLE(AActor_GetNetMode, "AActor::GetNetMode")
{
    return Memcury::Scanner::FindPatterns({ "E8 ? ? ? ? 8D 48 FE", "E8 ? ? ? ? 44 8B F8 41 83 FF 03", "E8 ? ? ? ? 83 F8 ? 7C ? FF C7" })
        .RelativeOffset(1)
        .GetAs<void*>();
}

DEFINE_MEMORY_VARIABLE(UWorld_GetNetMode, "UWorld::GetNetMode")
{
    return Memcury::Scanner::FindStringRef(L"STAT_Fort_TickCharacterAnim", 0, false)
        .ScanFor({ 0xE8 })
        .RelativeOffset(1)
        .GetAs<void*>();
}

DEFINE_MEMORY_VARIABLE(FCurlHttpRequest_ProcessRequest, "FCurlHttpRequest::ProcessRequest")
{
    if (GetEngineVersion() >= 4.26)
    {
        auto StringRef = Memcury::Scanner::FindStringRef(L"STAT_FCurlHttpRequest_ProcessRequest");
        if (StringRef.IsValid())
        {
            return StringRef
                .ScanFor({ 0x40, 0x53 }, false)
                .GetAs<void*>();
        }
    }

    auto StringRef = Memcury::Scanner::FindStringRef(L"Could not set libcurl options for easy handle, processing HTTP request failed. Increase verbosity for additional information.");
    if (StringRef.IsValid())
    {
        return StringRef
            .ScanFor({ 0x48, 0x81, 0xEC }, false)
            .ScanFor({ 0x40 }, false)
            .GetAs<void*>();
    }

    return NULL;
}

DEFINE_MEMORY_VARIABLE(UMcpProfileGroup_SendRequestNow, "UMcpProfileGroup::SendRequestNow")
{
    return Memcury::Scanner::FindStringRef(L"MCP-Profile: Dispatching request to %s")
        .ScanFor({ 0x48, 0x89, 0x5C }, false)
        .GetAs<void*>();
}

DEFINE_MEMORY_VARIABLE(UFortEngine_Init, "UFortEngine::Init")
{
    auto addr = Memcury::Scanner::FindPattern("48 89 5C 24 ? 57 48 83 EC 20 48 8B DA 48 8B F9 48 8D 15 ? ? ? ? 41 B8");
    if (!addr.IsValid())
        addr = Memcury::Scanner::FindPattern("48 89 5C 24 ? 57 48 83 EC 60 48 8B DA 48 8B F9 48 8D 15");

    return addr.GetAs<void*>();
}

DEFINE_MEMORY_OFFSET(UEngine_GetMaxTickRateVFI, "UEngine::GetMaxTickRate")
{
    auto StringRef = Memcury::Scanner::FindStringRef(L"GETMAXTICKRATE");
    if (StringRef.IsValid())
    {
        auto VFI = *StringRef
            .ScanFor({ 0x48, 0x8B })
            .ScanFor({ 0xE8 })
            .RelativeOffset(1)
            .ScanFor({ 0xFF, 0x90 })
            .AbsoluteOffset(2)
            .GetAs<int*>() / 8;

        if (VFI >= 60 && VFI <= 100)
            return VFI;
        else
        {
            VFI = *StringRef
                .ScanFor({ 0xFF, 0x90 })
                .AbsoluteOffset(2)
                .GetAs<int*>() / 8;

            return VFI;
        }
    }

    return -1;
}

DEFINE_MEMORY_VARIABLE(UFortReplicationGraph_ServerReplicateActors, "UFortReplicationGraph::ServerReplicateActors")
{
    return Memcury::Scanner::FindStringRef(L"NET_PrepareReplication")
        .ScanFor({ 0x4C, 0x8B, 0xDC }, false)
        .GetAs<void*>();
}

DEFINE_MEMORY_VARIABLE(UFortReplicationGraph_OnPlayerStateSquadAssign, "UFortReplicationGraph::OnPlayerStateSquadAssign")
{
    return Memcury::Scanner::FindPatterns({ "48 89 5C 24 ? 48 89 74 24 ? 48 89 54 24 ? 57 48 83 EC 20 48 8B 02", "48 89 54 24 ? 53 55 56 48 81 EC" })
        .GetAs<void*>();
}

DEFINE_MEMORY_VARIABLE(UNetDriver_TickFlush, "UNetDriver::TickFlush")
{
    return Memcury::Scanner::FindStringRef(L"STAT_NetTickFlush")
        .ScanFor({ 0x4C, 0x8B, 0xDC }, false)
        .GetAs<void*>();
}

DEFINE_MEMORY_VARIABLE(UKismetArrayLibrary_Array_Add, "UKismetArrayLibrary::Array_Add")
{
    return Memcury::Scanner::FindPatterns({ "E8 ? ? ? ? 41 FF CE FF C3 D1 C6", "E8 ? ? ? ? FF CE FF C5" })
        .RelativeOffset(1)
        .GetAs<void*>();
}

DEFINE_MEMORY_VARIABLE(UKismetArrayLibrary_Array_Remove, "UKismetArrayLibrary::Array_Remove")
{
    return Memcury::Scanner::FindStringRef(L"Attempted to remove an item from an invalid index from array %s [%d/%d]!")
        .ScanFor({ 0x48, 0x85, 0xC9 }, false)
        .GetAs<void*>();
}

DEFINE_MEMORY_VARIABLE(FGameplayAbilitySpecCtor, "FGameplayAbilitySpecCtor")
{
    return Memcury::Scanner::FindPatterns({ "80 61 29 F8 48 8B 44 24", "48 8B 44 24 ? 80 61 29 F8 80", "80 61 31 FE 0F 57 C0 80" })
        .GetAs<void*>();
}

DEFINE_MEMORY_VARIABLE(UAbilitySystemComponent_GiveAbility, "UAbilitySystemComponent::GiveAbility")
{
    return Memcury::Scanner::FindStringRef(L"GiveAbilityAndActivateOnce called on ability %s on the client, not allowed!")
        .ScanFor({ 0xE8 })
        .RelativeOffset(1)
        .GetAs<void*>();
}

DEFINE_MEMORY_VARIABLE(UAbilitySystemComponent_InternalTryActivateAbility, "UAbilitySystemComponent::InternalTryActivateAbility")
{
    return Memcury::Scanner::FindStringRef(L"InternalTryActivateAbility called with invalid Handle! ASC: %s. AvatarActor: %s")
        .ScanFor({ 0x4C, 0x89, 0x4C }, false)
        .GetAs<void*>();
}

DEFINE_MEMORY_VARIABLE(UAbilitySystemComponent_ServerSetReplicatedTargetDataCancelled, "UAbilitySystemComponent::ServerSetReplicatedTargetDataCancelled")
{
    return Memcury::Scanner::FindPattern("48 89 5C ? ? 55 56 57 48 81 EC ? ? 00 00 41 0F 10 01 49 8B E9 48 63 DA F2 41 0F 10 49 ? 49 8B F8")
        .GetAs<void*>();
}

DEFINE_MEMORY_VARIABLE(UBuildingStructuralSupportSystem_CanAddBuildingActorClassToGrid, "UBuildingStructuralSupportSystem::CanAddBuildingActorClassToGrid")
{
    auto CanAddBuildingActorClassToGridAddr = Memcury::Scanner::FindPatterns({ "48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 41 56 48 83 EC 60 49 8B E9", "48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 41 56 48 83 EC 50 49 8B E9", "48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 57 41 56 41 57 48 83 EC 70 48 8B 1A 4D 8B F1 4D 8B F8 48 8B FA 48 8B F1 BD" }, false)
        .GetAs<void*>();

    if (!CanAddBuildingActorClassToGridAddr)
    {
        CanAddBuildingActorClassToGridAddr = Memcury::Scanner::FindPattern("E8 ? ? ? ? 85 C0 0F 85 ? ? ? ? 48 8D 4C 24 ? E8 ? ? ? ? C6 44 24")
            .RelativeOffset(1)
            .GetAs<void*>();
    }

    return CanAddBuildingActorClassToGridAddr;
}

DEFINE_MEMORY_VARIABLE(ABuildingSMActor_ReplaceBuildingActor, "ABuildingSMActor::ReplaceBuildingActor")
{
    auto StringRef = Memcury::Scanner::FindPattern("4C 8B DC 55 56 49 8D AB ? ? ? ? 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 85 ? ? ? ? 48 8B 85 ? ? ? ? 33 F6", false);
    if (StringRef.IsValid())
        return StringRef.GetAs<void*>();

    StringRef = Memcury::Scanner::FindStringRef(L"STAT_Fort_BuildingSMActorReplaceBuildingActor", 0, false);
    if (StringRef.IsValid())
    {
        return StringRef
            .ScanFor({ 0x55, 0x57 }, false)
            .ScanFor({ 0x8B }, false)
            .GetAs<uint8*>() - 1;
    }

    StringRef = Memcury::Scanner::FindPattern("4C 89 44 24 ? 55 56 57 41 55 41 56 41 57 48 8D AC 24 ? ? ? ? 48 81 EC ? ? ? ? 45");
    if (StringRef.IsValid())
    {
        return StringRef
            .GetAs<void*>();
    }

    return NULL;
}

DEFINE_MEMORY_VARIABLE(ABuildingSMActor_SetEditingPlayer, "ABuildingSMActor::SetEditingPlayer")
{
    return Memcury::Scanner::FindPattern("48 89 5C 24 ? 57 48 83 EC ? 80 B9 ? ? ? ? ? 48 8B FA 48 8B D9 75 ? 48 83 B9")
        .GetAs<void*>();
}

DEFINE_MEMORY_VARIABLE(AFortOctopusVehicle_UpdateAimTrace, "AFortOctopusVehicle::UpdateAimTrace")
{
    return Memcury::Scanner::FindStringRef("TowhookTrace", 0, false)
        .ScanFor({ 0x40, 0x55 }, false)
        .GetAs<void*>();
}

DEFINE_MEMORY_VARIABLE(UMcpProfile_HandleProfileUpdate, "UMcpProfile::HandleProfileUpdate")
{
    if (GetEngineVersion() >= 4.23)
    {
        return Memcury::Scanner::FindStringRef(L"UMcpProfile::HandleProfileUpdate updated lock expiration on Account %s for ProfileId %s to %s")
            .ScanFor({ 0x4C, 0x8B, 0xDC }, false)
            .GetAs<void*>();
    }

    auto Addr = Memcury::Scanner::FindStringRef(L"UMcpProfile::HandleProfileUpdate updated lock expiration on Account %s for ProfileId %s to %s", 0, false)
        .ScanFor({ 0x40, 0x55 }, false);

    if (!Addr.IsValid())
        Addr = Memcury::Scanner::FindStringRef(L"UMcpProfile::HandleProfileUpdate updated lock expiration for ProfileId %s to %s").ScanFor({ 0x40, 0x55 }, false);

    return Addr.GetAs<void*>();
}

DEFINE_MEMORY_VARIABLE(FOnlineIdentityMcp_AutoLogin, "FOnlineIdentityMcp::AutoLogin")
{
    return Memcury::Scanner::FindPattern("48 8B C4 55 56 48 8D A8 ? ? ? ? 48 81 EC ? ? ? ? 48 89 58 08 48 8B F1 48 89 78 10")
        .GetAs<void*>();
}

DEFINE_MEMORY_VARIABLE(FOnlineIdentityMcp_AddUserAccount, "FOnlineIdentityMcp::AddUserAccount")
{
    return Memcury::Scanner::FindPattern("E8 ? ? ? ? 48 8B 06 48 8D 55 70 45 8B C4")
        .RelativeOffset(1)
        .GetAs<void*>();
}

DEFINE_MEMORY_VARIABLE(FOnlineIdentityMcp_AuthClient, "FOnlineIdentityMcp::AuthClient")
{
    return Memcury::Scanner::FindPattern("E8 ? ? ? ? 8B 45 DC 48 8D 4D D8 F7 D0 21 45 E8 E8 ? ? ? ? 48 8B 45 E0 8B 4D EC 3B 48 18 7C C5")
        .RelativeOffset(1)
        .GetAs<void*>();
}

DEFINE_MEMORY_VARIABLE(AFortGameSessionDedicated_OnCreateSessionComplete, "AFortGameSessionDedicated::OnCreateSessionComplete")
{
    return Memcury::Scanner::FindStringRef(L"OnCreateSessionComplete %s bSuccess: %d")
        .ScanFor({ 0x4C, 0x8B }, false)
        .ScanFor({ 0x48 }, false)
        .GetAs<void*>();
}

DEFINE_MEMORY_VARIABLE(AFortPartyBeaconHost_HandleMatchAssignment, "AFortPartyBeaconHost::HandleMatchAssignment")
{
    auto Address = Memcury::Scanner::FindPatterns({ "E8 ? ? ? ? 88 44 24 60 49 8D 56 18", "E8 ? ? ? ? 44 0F B6 F8 88 44 24 40", "E8 ? ? ? ? 84 C0 0F 84 ? ? ? ? 80 3D ? ? ? ? ? 48 89 9C 24 ? ? ? ? 48 89 B4 24" }, false)
        .RelativeOffset(1)
        .GetAs<void*>();

    if (!Address)
    {
        return Memcury::Scanner::FindStringRef(L"MMS - configuring beacon for match assignment, MatchId: %s", 0, false)
            .ScanFor({ 0x84, 0xC0 }, false)
            .ScanFor({ 0xE8 }, false)
            .GetAs<void*>();
    }

    return Address;
}

DEFINE_MEMORY_VARIABLE(FMatchmakingServiceDedicatedServer_HandleWebSocketMessage, "FMatchmakingServiceDedicatedServer::HandleWebSocketMessage")
{
    return Memcury::Scanner::FindPattern("40 55 53 56 57 41 57 48 8D 6C 24 ? 48 81 EC ? ? ? ? 33 F6 48 8D 3D")
        .GetAs<void*>();
}

DEFINE_MEMORY_VARIABLE(AFortGameSessionDedicatedAthena_GetNumberOfPlayersInSquad, "AFortGameSessionDedicatedAthena::GetNumberOfPlayersInSquad")
{
    return Memcury::Scanner::FindStringRef(L"GetNumberOfPlayersInSquad failed to find a squad id %ui", 0, false)
        .ScanFor({ 0x48, 0x83 }, false)
        .GetAs<void*>();
}

DEFINE_MEMORY_VARIABLE(AFortPlayerControllerZone_ClientOnPawnDied, "AFortPlayerControllerZone::ClientOnPawnDied")
{
    if (GetEngineVersion() >= 4.26)
    {
        return Memcury::Scanner::FindPattern("E8 ? ? ? ? 4C 8B BC ? ? ? 00 00 0F 57 C0 0F 2F 03")
            .RelativeOffset(1)
            .GetAs<void*>();
    }

    if (GetEngineVersion() >= 4.23)
    {
        return Memcury::Scanner::FindPatterns({ "E8 ? ? ? ? 48 8B AC 24 ? ? ? ? 0F 57 C0", "E8 ? ? ? ? 48 8B B4 24 ? ? ? ? 0F 57 C0 0F 2F 03", "E8 ? ? ? ? 4C 8B BC 24 ? ? ? ? 0F 57 C0 0F 2F 03" })
            .RelativeOffset(1)
            .GetAs<void*>();
    }

    return Memcury::Scanner::FindPatterns({ "E8 ? ? ? ? 48 8B B4 24 ? ? 00 00 0F 57 C0", "E8 ? ? ? ? 48 8B AC 24 ? ? ? ? 0F 57 C0", "E8 ? ? ? ? 48 8B B4 24 ? ? ? ? 0F 57 C0 0F 2F 03", "E8 ? ? ? ? 4C 8B BC 24 ? ? ? ? 0F 57 C0 0F 2F 03" })
        .RelativeOffset(1)
        .GetAs<void*>();
}

DEFINE_MEMORY_VARIABLE(AFortPlayerPawn_OnRep_InVehicle, "AFortPlayerPawn::OnRep_InVehicle")
{
    return Memcury::Scanner::FindStringRef(L"AFortPlayerPawn::OnRep_InVehicle: Character movement component isn't valid!")
        .ScanFor({ 0x40, 0x55 }, false)
        .GetAs<void*>();
}

DEFINE_MEMORY_VARIABLE(AFortPawn_StopPerformingNativeAction, "AFortPawn::StopPerformingNativeAction")
{
    return Memcury::Scanner::FindStringRef(L"Native Action Role(%s) Can't stop %s, not active!", 0, false)
        .ScanFor({ 0x48, 0x8B, 0xC4 }, false)
        .GetAs<void*>();
}

DEFINE_MEMORY_VARIABLE(UAbilitySystemComponent_CancelAbilities, "UAbilitySystemComponent::CancelAbilities")
{
    return Memcury::Scanner::FindPatterns({ "4C 89 4C 24 ? 48 89 4C 24 ? 53 41 54", "48 89 6C ? ? 56 41 54 41 55 41 56 41 57 48 83 EC ? 4C 8B F2" })
        .GetAs<void*>();
}

DEFINE_MEMORY_VARIABLE(FWeakObjectPtr_Get, "FWeakObjectPtr::Get")
{
    return Memcury::Scanner::FindStringRef(L"FastForwardLevels - NewStreamingLevel found in Pending list! %s")
        .ScanFor({ 0x48, 0x03 })
        .ScanFor({ 0xE8 })
        .RelativeOffset(1)
        .GetAs<void*>();
}

DEFINE_MEMORY_VARIABLE(FWeakObjectPtr_FWeakObjectPtr, "FWeakObjectPtr::FWeakObjectPtr")
{
    return Memcury::Scanner::FindStringRef(L"GameStateClass is not set, falling back to AGameState.", 0, false)
        .ScanFor({ 0x48, 0x85 })
        .ScanFor({ 0xE8 })
        .RelativeOffset(1)
        .GetAs<void*>();
}

DEFINE_MEMORY_VARIABLE(FFrame_StepExplicitProperty, "FFrame::StepExplicitProperty")
{
    return Memcury::Scanner::FindPattern("41 8B 40 ? 4D 8B C8")
        .GetAs<void*>();
}

DEFINE_MEMORY_VARIABLE(FFrame_Step, "FFrame::Step")
{
    return Memcury::Scanner::FindPattern("48 8B 41 20 4C 8B D2 48 8B D1 44 0F B6 08 48 FF")
        .GetAs<void*>();
}

DEFINE_MEMORY_VARIABLE(AFortPlayerState_ApplyCharacterCustomization, "AFortPlayerState::ApplyCharacterCustomization")
{
    return Memcury::Scanner::FindPattern("E8 ? ? ? ? E9 ? ? ? ? 4C 89 B4 24 ? ? ? ? FF 90 ? ? ? ?")
        .RelativeOffset(1)
        .GetAs<void*>();
}

DEFINE_MEMORY_VARIABLE(AFortPlayerStateAthena_ToDeathCause, "AFortPlayerStateAthena::ToDeathCause")
{
    return Memcury::Scanner::FindPattern("E8 ? ? ? ? 48 8B CF 88 44 24 69", false)
        .RelativeOffset(1)
        .GetAs<void*>();
}

DEFINE_MEMORY_VARIABLE(UAthenaNavSystem_Build, "UAthenaNavSystem::Build")
{
    return Memcury::Scanner::FindPattern("48 89 5C 24 ? 57 48 83 EC ? 48 8B 79 ? 48 8B D9 48 85 FF 74 ? E8 ? ? ? ? 48 8B 4F ? 4C 8D 40 ? 48 63 40 ? 3B 41 ? 7F ? 48 8B D0 48 8B 41 ? 4C 39 04 D0 75 ? 48 8B CF E8 ? ? ? ? 8B 43")
        .GetAs<void*>();
}

DEFINE_MEMORY_VARIABLE(ABuildingGameplayActorSpawnMachine_ResurrectPlayer, "ABuildingGameplayActorSpawnMachine::ResurrectPlayer")
{
    return Memcury::Scanner::FindPattern("48 8D 05 ? ? ? ? 33 F6 48 89 44 24 ? 49 8B CE")
        .AbsoluteOffset(2)
        .GetAs<void*>();
}

DEFINE_MEMORY_VARIABLE(UFortCreativeRealEstatePlotItem_ParseAttributes, "UFortCreativeRealEstatePlotItem::ParseAttributes")
{
    return Memcury::Scanner::FindPattern("48 8B C4 48 89 50 ? 48 89 48 ? 55 53 48 8D 68 ? 48 81 EC ? ? ? ? 48 89 70 ? 33 DB")
        .GetAs<void*>();
}

DEFINE_MEMORY_VARIABLE(UFortLevelSaveComponent_ResetLevelRecord, "UFortLevelSaveComponent::ResetLevelRecord")
{
    return Memcury::Scanner::FindStringRef(L"ResetLevelRecord requested")
        .ScanFor({ 0x40, 0x55 }, false)
        .GetAs<void*>();
}

DEFINE_MEMORY_VARIABLE(UFortLevelSaveComponent_LoadPlaysetAsync, "UFortLevelSaveComponent::LoadPlaysetAsync")
{
    return Memcury::Scanner::FindPattern("40 55 53 56 57 41 54 41 56 41 57 48 8D 6C 24 ? 48 81 EC ? ? ? ? 4C 8B B1")
        .GetAs<void*>();
}

DEFINE_MEMORY_VARIABLE(AFortMinigameSettingsBuilding_BeginPlay, "AFortMinigameSettingsBuilding::BeginPlay")
{
    return Memcury::Scanner::FindPattern("40 55 57 41 56 41 57 48 8B EC 48 83 EC ? 80 3D")
        .GetAs<void*>();
}

DEFINE_MEMORY_VARIABLE(AFortMinigame_SetState, "AFortMinigame::SetState")
{
    return Memcury::Scanner::FindPattern("40 55 53 56 57 48 8D AC 24 ? ? ? ? 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 45 ? 33 F6")
        .GetAs<void*>();
}

DEFINE_MEMORY_VARIABLE(UFortLevelSaveComponent_GetSidecar, "UFortLevelSaveComponent::GetSidecar")
{
    return Memcury::Scanner::FindPattern("40 53 48 83 EC ? 48 8B 99 ? ? ? ? 48 85 DB 75")
        .GetAs<void*>();
}

DEFINE_MEMORY_VARIABLE(UFortLevelSaveComponent_LoadFromDssAsync, "UFortLevelSaveComponent::LoadFromDssAsync")
{
    return Memcury::Scanner::FindPattern("48 8B C4 88 50 ? 55 53 41 55")
        .GetAs<void*>();
}

DEFINE_MEMORY_OFFSET(AFortOctopusVehicle_TowHookProjectile, "AFortOctopusVehicle::TowHookProjectile")
{
    auto addr = Memcury::Scanner::FindPattern("E8 ? ? ? ? 33 D2 48 8D 4C 24 ? E8 ? ? ? ? 44 8D 47 01")
        .RelativeOffset(1)
        .ScanFor({ 0x89, 0x83 })
        .AbsoluteOffset(2);

    if (!addr.IsValid())
        return 0x0;

    return *addr.GetAs<uint32*>();
}

DEFINE_MEMORY_OFFSET(AFortOctopusVehicle_TowhookAttachedComponent, "AFortOctopusVehicle::TowhookAttachedComponent")
{
    auto addr = Memcury::Scanner::FindStringRef("TowhookTrace", false, 0)
        .ScanFor({ 0x40, 0x55 }, false)
        .ScanFor({ 0x81, 0xC1 })
        .AbsoluteOffset(2);

    if (!addr.IsValid())
        return 0x0;

    return *addr.GetAs<uint32*>();
}

DEFINE_MEMORY_OFFSET(AFortOctopusVehicle_LocalAttachState, "AFortOctopusVehicle::LocalAttachState")
{
    if (GetEngineVersion() <= 4.22 || AFortOctopusVehicle::StaticClass() == NULL || AFortOctopusVehicle::StaticClass()->GetProperty(L"ReplicatedAttachState") == NULL)
        return -1;

    return AFortOctopusVehicle::StaticClass()->GetProperty(L"ReplicatedAttachState")->OffsetInternal + FNetTowhookAttachState::StaticStruct()->PropertiesSize;
}

DEFINE_MEMORY_OFFSET(IHttpRequest_NetMode, "IHttpRequest::NetMode")
{
    if (GetEngineVersion() >= 4.23)
        return -1;

    if (GetEngineVersion() >= 4.22)
        return 0x28;

    return (uint32)*Memcury::Scanner::FindStringRef(L"MCP-Profile: Dispatching request to %s")
        .ScanFor({ 0x8B, 0x4F })
        .AbsoluteOffset(2)
        .GetAs<uint8*>();
}

DEFINE_MEMORY_OFFSET(UField_Next, "UField::Next")
{
    return 0x28;
}

DEFINE_MEMORY_OFFSET(UStruct_SuperStruct, "UStruct::SuperStruct")
{
    auto Class = FindObject<UClass>(L"/Script/Engine.Actor");
    auto SuperStruct = FindObject<UClass>(L"/Script/CoreUObject.Object");

    return FindOffset(Class, SuperStruct);
}

DEFINE_MEMORY_OFFSET(UStruct_Children, "UStruct::Children")
{
    if (GetEngineVersion() <= 4.21)
        return 0x38;

    auto Class = FindObject<UClass>(L"/Script/Engine.Actor");
    auto Function = FindObject<UFunction>(L"/Script/Engine.Actor.WasRecentlyRendered");
    auto Offset = FindOffset(Class, Function);

    if (Offset == -1)
    {
        return 0x48;
    }

    return Offset;
}

DEFINE_MEMORY_OFFSET(UStruct_PropertiesSize, "UStruct::PropertiesSize")
{
    if (GetEngineVersion() >= 4.25)
        return 0x58;

    auto Struct = FindObject<UStruct>(L"/Script/CoreUObject.Transform");
    auto Offset = FindOffset(Struct, sizeof(FTransform));

    if (Offset == -1)
    {
        return UKismetMemoryLibrary::Get(L"UStruct::Children") + 8;
    }

    return Offset;
}

DEFINE_MEMORY_OFFSET(UClass_ClassDefaultObject, "UClass::ClassDefaultObject")
{
    auto Class = FindObject<UClass>(L"/Script/Engine.Actor");
    auto DefaultObject = FindObject<UObject>(L"/Script/Engine.Default__Actor");

    return FindOffset(Class, DefaultObject);
}

DEFINE_MEMORY_OFFSET(UProperty_Offset_Internal, "UProperty::Offset_Internal")
{
    return 0x44;
}

DEFINE_MEMORY_OFFSET(UBoolProperty_ByteOffset, "UBoolProperty::ByteOffset")
{
    return 0x71;
}

DEFINE_MEMORY_OFFSET(UBoolProperty_ByteMask, "UBoolProperty::ByteMask")
{
    return 0x72;
}

DEFINE_MEMORY_OFFSET(UBoolProperty_FieldMask, "UBoolProperty::FieldMask")
{
    return 0x73;
}

DEFINE_MEMORY_OFFSET(UFunction_Func, "UFunction::Func")
{
    if (GetEngineVersion() <= 4.21)
        return 0xB0;

    if (GetEngineVersion() >= 4.26)
        return 0xD8;

    if (GetEngineVersion() >= 4.25)
        return 0xF0;

    return 0xC0;
}

DEFINE_MEMORY_OFFSET(UFunction_ParmsSize, "UFunction::ParmsSize")
{
    auto Function = FindObject<UFunction>(L"/Script/Engine.DecalComponent.SetFadeOut");

    return FindOffset<uint16>(Function, 0x9);
}

DEFINE_MEMORY_OFFSET(FFrame_PropertyChainForCompiledIn, "FFrame::PropertyChainForCompiledIn")
{
    return 0x80;
}

DEFINE_MEMORY_OFFSET(UStruct_ChildProperties, "UStruct::ChildProperties")
{
    return 0x50;
}

DEFINE_MEMORY_OFFSET(FProperty_Offset_Internal, "FProperty::Offset_Internal")
{
    return 0x4C;
}

DEFINE_MEMORY_OFFSET(FField_Next, "FField::Next")
{
    return 0x20;
}

DEFINE_MEMORY_OFFSET(FField_Name, "FField::Name")
{
    return 0x28;
}

DEFINE_MEMORY_OFFSET(FStructProperty_Struct, "FStructProperty::Struct")
{
    return 0x78;
}

DEFINE_MEMORY_OFFSET(FBoolProperty_ByteOffset, "FBoolProperty::ByteOffset")
{
    return 0x79;
}

DEFINE_MEMORY_OFFSET(FBoolProperty_ByteMask, "FBoolProperty::ByteMask")
{
    return 0x7A;
}

DEFINE_MEMORY_OFFSET(FBoolProperty_FieldMask, "FBoolProperty::FieldMask")
{
    return 0x7B;
}