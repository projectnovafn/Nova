#include "Patches/FortLevelSaveComponentPatch.h"
#include <KismetDetoursLibrary.h>

static UObject* (*_GetSidecar)(UFortLevelSaveComponent*);

UObject* UFortLevelSaveComponentPatch::GetSidecarHook(UFortLevelSaveComponent* Component)
{
	return Component->GetSidecar();
}

void UFortLevelSaveComponentPatch::Init()
{
    _GetSidecar = UKismetMemoryLibrary::Get<decltype(_GetSidecar)>(L"UFortLevelSaveComponent::GetSidecar");

    /*UDetour* GetSidecarDetour = new UDetour();
    GetSidecarDetour->Init(_GetSidecar, GetSidecarHook);
    GetSidecarDetour->Commit();*/
}
