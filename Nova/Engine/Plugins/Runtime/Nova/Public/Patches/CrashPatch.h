#pragma once
#include "CoreMinimal.h"
#include "Engine/Brush.h"

class UDetour;

class CrashPatch
{
private:
	static void LoadEngineFontsHook();
	static void CheckImageIntegrityHook();
	static void FilterCommandLineHook();
	static bool CheckPawnOverlapHook(ABrush*, void*);
	static void* ReplicatedDataMapHook(void*, void*, void*, void*);

public:
	static void PostInit();

	static void Init();
};