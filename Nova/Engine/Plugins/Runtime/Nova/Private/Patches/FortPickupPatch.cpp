#include "Patches/FortPickupPatch.h"
#include "KismetDetoursLibrary.h"

static void (*_OnAboutToEnterBackpack)(AFortPickup*);

UDetour* OnAboutToEnterBackpackDetour = NULL;

void AFortPickupPatch::OnAboutToEnterBackpackHook(AFortPickup* Pickup)
{
	Pickup->OnAboutToEnterBackpack();

	_OnAboutToEnterBackpack(Pickup);
}

void AFortPickupPatch::Init()
{
	_OnAboutToEnterBackpack = UKismetMemoryLibrary::Get<decltype(_OnAboutToEnterBackpack)>(L"AFortPickup::OnAboutToEnterBackpack");

	OnAboutToEnterBackpackDetour = new UDetour();
	OnAboutToEnterBackpackDetour->Init(_OnAboutToEnterBackpack, OnAboutToEnterBackpackHook);
	OnAboutToEnterBackpackDetour->Commit();
}