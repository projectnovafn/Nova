#pragma once
#include "Weapons/FortCreativeMoveTool.h"

class AFortCreativeMoveToolPatch
{
private:
	static void ServerStartInteractingHook(AFortCreativeMoveTool*, TArray<AActor*>&, FTransform);
	static void ServerDuplicateStartInteractingHook(AFortCreativeMoveTool*, TArray<AActor*>&, FTransform);

	static void ServerSpawnActorWithTransformHook(AFortCreativeMoveTool*, AActor*, FTransform&, bool, bool, bool, bool);

public:

	static void Init();
};