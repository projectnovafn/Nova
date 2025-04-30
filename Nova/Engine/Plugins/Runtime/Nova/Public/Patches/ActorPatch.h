#pragma once
#include "GameFramework/Actor.h"

class AActorPatch
{
	static ENetMode GetNetModeHook(AActor*);

public:
	static void Init();
};