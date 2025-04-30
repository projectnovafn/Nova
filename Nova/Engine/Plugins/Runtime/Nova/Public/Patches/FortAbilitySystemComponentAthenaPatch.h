#pragma once
#include "Athena/FortAbilitySystemComponentAthena.h"

class UFortAbilitySystemComponentAthenaPatch
{
private:

	static void ServerTryActivateAbilityHook(UFortAbilitySystemComponentAthena*, FGameplayAbilitySpecHandle, bool, FPredictionKey*);
	static void ServerTryActivateAbilityWithEventDataHook(UFortAbilitySystemComponentAthena*, FGameplayAbilitySpecHandle, bool, FPredictionKey*, FGameplayEventData);

public:

	static void Init();
};