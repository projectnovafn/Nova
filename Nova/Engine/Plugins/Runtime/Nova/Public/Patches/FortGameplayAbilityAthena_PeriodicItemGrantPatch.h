#pragma once
#include "Abilities/FortGameplayAbilityAthena_PeriodicItemGrant.h"
#include "UObject/Stack.h"

class UFortGameplayAbilityAthena_PeriodicItemGrantPatch
{
private:

	static void execStopItemAwardTimersHook(UFortGameplayAbilityAthena_PeriodicItemGrant*, FFrame&);
	static void execStartItemAwardTimersHook(UFortGameplayAbilityAthena_PeriodicItemGrant*, FFrame&);

public:

	static void Init();
};