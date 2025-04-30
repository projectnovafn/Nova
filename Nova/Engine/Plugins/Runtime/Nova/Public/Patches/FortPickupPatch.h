#pragma once
#include "Inventory/FortPickup.h"

class AFortPickupPatch
{
private:

	static void OnAboutToEnterBackpackHook(AFortPickup*);

public:

	static void Init();
};