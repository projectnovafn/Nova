#include "Patches/FortDecoToolPatch.h"

#include "KismetDetoursLibrary.h"

#include "Weapons/FortDecoTool_ContextTrap.h"
#include "Weapons/FortTrapTool.h"

static void (*_ServerSpawnDeco)(AFortDecoTool*, FVector, FRotator, ABuildingSMActor*, EBuildingAttachmentType);
static void (*_ServerCreateBuildingAndSpawnDeco)(AFortDecoTool*, FVector, FRotator, FVector, FRotator, EBuildingAttachmentType);

void AFortDecoToolPatch::ServerSpawnDecoHook(AFortDecoTool* DecoTool, FVector Location, FRotator Rotation, ABuildingSMActor* AttachedActor, EBuildingAttachmentType InBuildingAttachmentType)
{
	DecoTool->ServerSpawnDeco(Location, Rotation, AttachedActor, InBuildingAttachmentType);
}

void AFortDecoToolPatch::ServerCreateBuildingAndSpawnDecoHook(AFortDecoTool* DecoTool, FVector BuildingLocation, FRotator BuildingRotation, FVector Location, FRotator Rotation, EBuildingAttachmentType InBuildingAttachmentType)
{
	DecoTool->ServerCreateBuildingAndSpawnDeco(BuildingLocation, BuildingRotation, Location, Rotation, InBuildingAttachmentType);
}

void AFortDecoToolPatch::Init()
{
	UKismetDetoursLibrary::AddReflectedDetour<AFortDecoTool>(
		L"ServerSpawnDeco",
		EReflectedDetourType::VFSwap,
		ServerSpawnDecoHook,
		_ServerSpawnDeco,
		1
	);

	UKismetDetoursLibrary::AddReflectedDetour<AFortDecoTool>(
		L"ServerCreateBuildingAndSpawnDeco",
		EReflectedDetourType::VFSwap,
		ServerCreateBuildingAndSpawnDecoHook,
		_ServerCreateBuildingAndSpawnDeco,
		1
	);

	UKismetDetoursLibrary::AddReflectedDetour<AFortTrapTool>(
		L"ServerCreateBuildingAndSpawnDeco",
		EReflectedDetourType::VFSwap,
		ServerCreateBuildingAndSpawnDecoHook,
		_ServerCreateBuildingAndSpawnDeco,
		1
	);

	UKismetDetoursLibrary::AddReflectedDetour<AFortDecoTool_ContextTrap>(
		L"ServerCreateBuildingAndSpawnDeco",
		EReflectedDetourType::VFSwap,
		ServerCreateBuildingAndSpawnDecoHook,
		_ServerCreateBuildingAndSpawnDeco,
		1
	);
}