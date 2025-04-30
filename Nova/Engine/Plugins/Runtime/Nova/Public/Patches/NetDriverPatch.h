#pragma once
#include "Engine/NetDriver.h"

class UNetDriverPatch
{
private:
	static void TickFlushHook(UNetDriver*, float);

public:

	static void Init();
};