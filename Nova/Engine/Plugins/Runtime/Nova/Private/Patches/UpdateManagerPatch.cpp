#include "Patches/UpdateManagerPatch.h"
#include "KismetDetoursLibrary.h"

static void (*_StartPatchCheck)(UUpdateManager*);

UDetour* StartPatchCheckDetour = NULL;

void UUpdateManagerPatch::StartPatchCheckHook(UUpdateManager* UpdateManager)
{
	UpdateManager->StartPatchCheck();
}

void UUpdateManagerPatch::Init()
{
	_StartPatchCheck = UKismetMemoryLibrary::Get<decltype(_StartPatchCheck)>(L"UUpdateManager::StartPatchCheck");

	StartPatchCheckDetour = new UDetour();
	StartPatchCheckDetour->Init(_StartPatchCheck, StartPatchCheckHook);
	StartPatchCheckDetour->Commit();
}