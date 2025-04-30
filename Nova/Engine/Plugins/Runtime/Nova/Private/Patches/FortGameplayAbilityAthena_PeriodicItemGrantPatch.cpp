#include "Patches/FortGameplayAbilityAthena_PeriodicItemGrantPatch.h"
#include "KismetDetoursLibrary.h"

static void (*_execStopItemAwardTimers)(UFortGameplayAbilityAthena_PeriodicItemGrant*, FFrame&);
static void (*_execStartItemAwardTimers)(UFortGameplayAbilityAthena_PeriodicItemGrant*, FFrame&);

void UFortGameplayAbilityAthena_PeriodicItemGrantPatch::execStopItemAwardTimersHook(UFortGameplayAbilityAthena_PeriodicItemGrant* GameplayAbility, FFrame& Stack)
{
	GameplayAbility->StopItemAwardTimers();

	_execStopItemAwardTimers(GameplayAbility, Stack);
}

void UFortGameplayAbilityAthena_PeriodicItemGrantPatch::execStartItemAwardTimersHook(UFortGameplayAbilityAthena_PeriodicItemGrant* GameplayAbility, FFrame& Stack)
{
	GameplayAbility->StartItemAwardTimers();

	_execStartItemAwardTimers(GameplayAbility, Stack);
}

void UFortGameplayAbilityAthena_PeriodicItemGrantPatch::Init()
{
	UKismetDetoursLibrary::AddReflectedDetour<UFortGameplayAbilityAthena_PeriodicItemGrant>(
		L"StopItemAwardTimers",
		EReflectedDetourType::ExecSwap,
		execStopItemAwardTimersHook,
		_execStopItemAwardTimers
	);

	UKismetDetoursLibrary::AddReflectedDetour<UFortGameplayAbilityAthena_PeriodicItemGrant>(
		L"StartItemAwardTimers",
		EReflectedDetourType::ExecSwap,
		execStartItemAwardTimersHook,
		_execStartItemAwardTimers
	);
}