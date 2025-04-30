#include "Nova.h"

#include "Patches.h"

#include "Core.h"
#include "CoreUObject.h"
#include "Engine.h"
#include "FortniteGame.h"

#include "Kismet/KismetSystemLibrary.h"

#include "Engine/World.h"
#include "KismetDetoursLibrary.h"

#include "vendor/memcury.h"

LPTOP_LEVEL_EXCEPTION_FILTER(*_SetUnhandledExceptionFilter)(LPTOP_LEVEL_EXCEPTION_FILTER) = SetUnhandledExceptionFilter;
LPTOP_LEVEL_EXCEPTION_FILTER SetUnhandledExceptionFilterHook(LPTOP_LEVEL_EXCEPTION_FILTER TopLevelExceptionFilter)
{
    auto Result = _SetUnhandledExceptionFilter(TopLevelExceptionFilter);

    if (FPlatformMisc::IsWithinImage(TopLevelExceptionFilter))
    {
        GNova->Loaded();
    }

    return Result;
}

bool UNova::UseLateInitialization()
{
    return UKismetSystemLibrary::GetFortniteVersion() == 10.40;
}

void UNova::InitGlobals()
{
    GIsServer = UKismetMemoryLibrary::Get<bool*>(L"GIsServer");
    GIsClient = UKismetMemoryLibrary::Get<bool*>(L"GIsClient");
}

void UNova::OnEngineInit()
{
    UKismetMemoryLibrary::LoadCache();

    CrashPatch::PostInit();

    UWorldPatch::Init();

    AFortGameModeAthenaPatch::Init();
    AFortPlayerControllerAthenaPatch::Init();
    AFortPhysicsPawnPatch::Init();
    AFortAthenaVehiclePatch::Init();
    AFortOctopusVehiclePatch::Init();
    AFortPlayerPawnAthenaPatch::Init();
    AFortPickupPatch::Init();
    AFortPlayerStateAthenaPatch::Init();
    AFortDecoToolPatch::Init();
    AFortGameSessionDedicatedPatch::Init();
    AFortGameStateAthenaPatch::Init();
    AFortGameSessionDedicatedAthenaPatch::Init();
    AFortAthenaVehicleSpawnerPatch::Init();
    AFortWeaponPatch::Init();
    AFortWeaponRangedMountedCannonPatch::Init();

    AFortAthenaCreativePortalPatch::Init();
    AFortMinigamePatch::Init();
    AFortCreativeMoveToolPatch::Init();
    AFortMinigameSettingsBuildingPatch::Init();

    FCurlHttpRequestPatch::Init();

    UFortAbilitySystemComponentAthenaPatch::Init();
    UFortGameplayAbilityAthena_PeriodicItemGrantPatch::Init();

    UFortControllerComponent_InteractionPatch::Init();
    UFortControllerComponent_AircraftPatch::Init();

    UFortKismetLibraryPatch::Init();

    UAthenaMarkerComponentPatch::Init();
    UUpdateManagerPatch::Init();
    UNetDriverPatch::Init();
    UNetConnectionPatch::Init();

    ABuildingSMActorPatch::Init();
    ABuildingGameplayActorSpawnMachinePatch::Init();
    AActorPatch::Init();

    McpPatch::PostInit();

    FMatchmakingServiceDedicatedServerPatch::Init();

    LoggingPatch::Init();

    if (UseLateInitialization())
        UFortUIStateWidget_LoginPatch::Init();

	UFortMissionLibraryPatch::Init();
	UFortLevelSaveComponentPatch::Init();
}

void UNova::Loaded()
{
    GLogConsole->Show(true);
    ExceptionFilterDetour->Remove();
    this->InitGlobals();

    UFortEnginePatches::Init();
    McpPatch::Init();

    CrashPatch::Init();
}

void UNova::Init()
{
    ExceptionFilterDetour = new UDetour();
    ExceptionFilterDetour->Init(_SetUnhandledExceptionFilter, SetUnhandledExceptionFilterHook);
    ExceptionFilterDetour->Commit();

    FPlatformMisc::ResumeMainThread();
}