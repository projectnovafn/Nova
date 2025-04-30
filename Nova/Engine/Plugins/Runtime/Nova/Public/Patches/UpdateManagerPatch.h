#pragma once
#include "UpdateManager.h"

class UUpdateManagerPatch
{
private:
	static void StartPatchCheckHook(UUpdateManager*);
	static void SetUpdateStateHook(UUpdateManager*, EUpdateState);

public:

	static void Init();
};