#include "Patches/FortUIStateWidget_LoginPatch.h"

#include "KismetDetoursLibrary.h"
#include "Engine\GameplayStatics.h"
#include "Engine\World.h"
#include "FortGlobals.h"

static void* (*_HandleLoginRequestComplete)(void*, uint8, void*);

UDetour* HandleLoginRequestCompleteDetour = NULL;

void* UFortUIStateWidget_LoginPatch::HandleLoginRequestCompleteHook(void* a1, uint8 Result, void* a3)
{
	if (Result == 27 ||
		Result == 0)
	{
		HandleLoginRequestCompleteDetour->Remove();

		*GIsServer = true;
		*GIsClient = false;

		GWorld->GameInstance->LocalPlayers.RemoveAt(0);

		if (auto FortGlobals = GetDefaultObject<UFortGlobals>())
		{
			UGameplayStatics::OpenLevel(GWorld, *FortGlobals->GetBRMapShortName(), true, {});
		}
	}

	return _HandleLoginRequestComplete(a1, Result, a3);
}

void UFortUIStateWidget_LoginPatch::Init()
{
	_HandleLoginRequestComplete = UKismetMemoryLibrary::Get<decltype(_HandleLoginRequestComplete)>(L"UFortUIStateWidget_Login::HandleLoginRequestComplete");

	HandleLoginRequestCompleteDetour = new UDetour();
	HandleLoginRequestCompleteDetour->Init(_HandleLoginRequestComplete, HandleLoginRequestCompleteHook);
	HandleLoginRequestCompleteDetour->Commit();
}