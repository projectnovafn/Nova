#pragma once
#include "Core.h"

class UFortUIStateWidget_LoginPatch
{
private:
	static void* HandleLoginRequestCompleteHook(void*, uint8, void*);

public:

	static void Init();
};