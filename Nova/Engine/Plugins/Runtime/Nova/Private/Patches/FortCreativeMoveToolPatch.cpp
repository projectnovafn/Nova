#include "Patches/FortCreativeMoveToolPatch.h"

#include "KismetDetoursLibrary.h"

void (*_ServerStartInteracting)(AFortCreativeMoveTool*, TArray<AActor*>&, FTransform);
void (*_ServerDuplicateStartInteracting)(AFortCreativeMoveTool*, TArray<AActor*>&, FTransform);
void (*_ServerSpawnActorWithTransform)(AFortCreativeMoveTool*, AActor*, FTransform, bool, bool, bool, bool);

void AFortCreativeMoveToolPatch::ServerStartInteractingHook(AFortCreativeMoveTool* MoveTool, TArray<AActor*>& Actors, FTransform DragStart)
{
	MoveTool->ServerStartInteracting(Actors, DragStart);
}

void AFortCreativeMoveToolPatch::ServerDuplicateStartInteractingHook(AFortCreativeMoveTool* MoveTool, TArray<AActor*>& Actors, FTransform DragStart)
{
	MoveTool->ServerDuplicateStartInteracting(Actors, DragStart);
}

void AFortCreativeMoveToolPatch::ServerSpawnActorWithTransformHook(AFortCreativeMoveTool* MoveTool, AActor* ActorToSpawn, FTransform& TargetTransform, bool bAllowOverlap, bool bAllowGravity, bool bIgnoreStructuralIssues, bool bForPreviewing)
{
	MoveTool->ServerSpawnActorWithTransform(ActorToSpawn, TargetTransform, bAllowOverlap, bAllowGravity, bIgnoreStructuralIssues, bForPreviewing);
}

void AFortCreativeMoveToolPatch::Init()
{
	UKismetDetoursLibrary::AddReflectedDetour<AFortCreativeMoveTool>(
		L"ServerStartInteracting",
		EReflectedDetourType::VFSwap,
		ServerStartInteractingHook,
		_ServerStartInteracting,
		1
	);

	UKismetDetoursLibrary::AddReflectedDetour<AFortCreativeMoveTool>(
		L"ServerDuplicateStartInteracting",
		EReflectedDetourType::VFSwap,
		ServerDuplicateStartInteractingHook,
		_ServerDuplicateStartInteracting,
		1
	);

	UKismetDetoursLibrary::AddReflectedDetour<AFortCreativeMoveTool>(
		L"ServerSpawnActorWithTransform",
		EReflectedDetourType::VFSwap,
		ServerSpawnActorWithTransformHook,
		_ServerSpawnActorWithTransform,
		1
	);
}