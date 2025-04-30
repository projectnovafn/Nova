#include "Patches/FortEnginePatch.h"
#include "KismetDetoursLibrary.h"

static void (*_Init)(UFortEngine*, void*);

UDetour* InitDetour = NULL;

void UFortEnginePatches::InitHook(UFortEngine* Engine, void* EngineLoop)
{
	static auto GetMaxTickRateVFI = UKismetMemoryLibrary::Get(L"UEngine::GetMaxTickRate");

	if (GetMaxTickRateVFI != -1)
		Engine->VTable[GetMaxTickRateVFI] = UFortEnginePatches::GetMaxTickRateHook;

	Engine->Init(EngineLoop);

	_Init(Engine, EngineLoop);

	InitDetour->Remove();
}

float UFortEnginePatches::GetMaxTickRateHook(UFortEngine* Engine, float DeltaTime, bool bAllowFrameSmoothing)
{
	return Engine->GetMaxTickRate(DeltaTime, bAllowFrameSmoothing);
}

void UFortEnginePatches::Init()
{
	_Init = UKismetMemoryLibrary::Get<decltype(_Init)>(L"UFortEngine::Init");

	InitDetour = new UDetour();
	InitDetour->Init(_Init, InitHook, EDetourType::VFSwap);
	InitDetour->Commit();
}