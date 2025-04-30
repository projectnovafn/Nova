#pragma once
#include "Patches/CrashPatch.h"
#include "KismetDetoursLibrary.h"

#include "Kismet/KismetSystemLibrary.h"

#include "Nova.h"

#include <vendor\memcury.h>

static void (*_CheckImageIntegrity)();
static void (*_LoadEngineFonts)();
static void (*_FilterCommandLine)();
static bool (*_CheckPawnOverlap)(ABrush*, void*);
static void* (*_ReplicatedDataMap)(void*, void*, void*, void*);

UDetour* LoadEngineFontsDetour = NULL;
UDetour* CheckImageIntegrityDetour = NULL;
UDetour* FilterCommandLineDetour = NULL;
UDetour* CheckPawnOverlapDetour = NULL;
UDetour* ReplicatedDataMapDetour = NULL;

const uint8 IntegrityCheckPatchBytes[] = { 0x90, 0xB8, 0x00, 0x00, 0x00, 0x00, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0xE9 };
const uint8 BroadcastPatchBytes[] = { 0x90, 0x90, 0x90, 0x90, 0x90 };

void CrashPatch::LoadEngineFontsHook()
{
    UE_LOG(LogPatches, Log, TEXT("CrashPatch::LoadEngineFontsHook"));  

    LoadEngineFontsDetour->Remove();
}

void CrashPatch::CheckImageIntegrityHook()
{
    UE_LOG(LogPatches, Log, TEXT("CrashPatch::CheckImageIntegrityHook"));

    while (true)
    {
        auto IntegrityPatch = Memcury::Scanner::FindPattern("8B 05 ? ? ? ? 33 05 ? ? ? ? 01 05 ? ? ? ? E9", false)
            .GetAs<void*>();

        if (IntegrityPatch == NULL)
            break;

        FMemory::Memcpy(IntegrityPatch, IntegrityCheckPatchBytes, sizeof(IntegrityCheckPatchBytes));
    }
}

void CrashPatch::FilterCommandLineHook()
{
    UE_LOG(LogPatches, Log, TEXT("CrashPatch::FilterCommandLineHook"));

    FilterCommandLineDetour->Remove();
}

bool CrashPatch::CheckPawnOverlapHook(ABrush* Brush, void* a2)
{
    if (Brush->BrushComponent == NULL)
        return false;

    return _CheckPawnOverlap(Brush, a2);
}

void* CrashPatch::ReplicatedDataMapHook(void* a1, void* a2, void* a3, void* a4)
{
    if (IsBadReadPtr(*(void**)a1, 8) || *((DWORD*)a1 + 13))
    {
        FMemory::Memzero(a1, sizeof(TMap<void*, void*>));
    }

    return _ReplicatedDataMap(a1, a2, a3, a4);
}

void CrashPatch::PostInit()
{
    _ReplicatedDataMap = UKismetMemoryLibrary::Get<decltype(_ReplicatedDataMap)>(L"ReplicatedDataMap");
    _LoadEngineFonts = UKismetMemoryLibrary::Get<decltype(_LoadEngineFonts)>(L"LoadEngineFonts");

    if (UKismetSystemLibrary::GetEngineVersionNum() == 4.21)
    {
        ReplicatedDataMapDetour = new UDetour();
        ReplicatedDataMapDetour->Init(_ReplicatedDataMap, ReplicatedDataMapHook);
        ReplicatedDataMapDetour->Commit();
    }

    if (!GNova->UseLateInitialization())
    {
        LoadEngineFontsDetour = new UDetour();
        LoadEngineFontsDetour->Init(_LoadEngineFonts, LoadEngineFontsHook);
        LoadEngineFontsDetour->Commit();
    }

    if (auto Broadcast = UKismetMemoryLibrary::Get<uint8*>(L"Broadcast"))
    {
        FMemory::Memcpy(Broadcast, BroadcastPatchBytes, sizeof(BroadcastPatchBytes));
    }

    if (UKismetSystemLibrary::GetEngineVersionNum() >= 4.25)
    {
        *UKismetMemoryLibrary::Get<uint8*>(L"AFortGameModeAthena::StartMatchAbandonProcess") = 0xC3;
    }

    if (auto RequestExit = UKismetMemoryLibrary::Get<uint8*>(L"FWindowsPlatformMisc::RequestExit"))
        *RequestExit = 0xC3;
}

void CrashPatch::Init()
{
    _FilterCommandLine = UKismetMemoryLibrary::Get<decltype(_FilterCommandLine)>(L"FilterCommandLine");
    _CheckImageIntegrity = UKismetMemoryLibrary::Get<decltype(_CheckImageIntegrity)>(L"CheckImageIntegrity");
    _CheckPawnOverlap = UKismetMemoryLibrary::Get<decltype(_CheckPawnOverlap)>(L"CheckPawnOverlap");

    FilterCommandLineDetour = new UDetour();
    FilterCommandLineDetour->Init(_FilterCommandLine, FilterCommandLineHook);
    FilterCommandLineDetour->Commit();

    CheckImageIntegrityDetour = new UDetour();
    CheckImageIntegrityDetour->Init(_CheckImageIntegrity, CheckImageIntegrityHook);
    CheckImageIntegrityDetour->Commit();

    CheckPawnOverlapDetour = new UDetour();
    CheckPawnOverlapDetour->Init(_CheckPawnOverlap, CheckPawnOverlapHook);
    CheckPawnOverlapDetour->Commit();

    *UKismetMemoryLibrary::Get<uint8*>(L"ChangeGameSessionId") = 0xC3;
    *UKismetMemoryLibrary::Get<uint8*>(L"DedicatedServerReady") = 0x85;

    if (auto CollectGarbageInternal = UKismetMemoryLibrary::Get<uint8*>(L"CollectGarbageInternal"))
    {
        *CollectGarbageInternal = 0xC3; // sigh
    }
}