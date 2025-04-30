#pragma once

#include "Weapons/FortDecoTool.h"

class ABuildingSMActor;

class AFortDecoToolPatch
{
private:

	static void ServerSpawnDecoHook(AFortDecoTool*, FVector, FRotator, ABuildingSMActor*, EBuildingAttachmentType);
	static void ServerCreateBuildingAndSpawnDecoHook(AFortDecoTool*, FVector, FRotator, FVector, FRotator, EBuildingAttachmentType);

public:

	static void Init();
};