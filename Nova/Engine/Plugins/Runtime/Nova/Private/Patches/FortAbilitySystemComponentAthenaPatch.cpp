#include "Patches/FortAbilitySystemComponentAthenaPatch.h"
#include "KismetDetoursLibrary.h"

static void (*_ServerTryActivateAbility)(UFortAbilitySystemComponentAthena*, FGameplayAbilitySpecHandle, bool, FPredictionKey*);
static void (*_ServerTryActivateAbilityWithEventData)(UFortAbilitySystemComponentAthena*, FGameplayAbilitySpecHandle, bool, FPredictionKey*, void*);

void UFortAbilitySystemComponentAthenaPatch::ServerTryActivateAbilityHook(UFortAbilitySystemComponentAthena* AbilitySystemComponent, FGameplayAbilitySpecHandle Handle, bool InputPressed, FPredictionKey* PredictionKey)
{
	AbilitySystemComponent->ServerTryActivateAbility(Handle, InputPressed, PredictionKey);
}

void UFortAbilitySystemComponentAthenaPatch::ServerTryActivateAbilityWithEventDataHook(UFortAbilitySystemComponentAthena* AbilitySystemComponent, FGameplayAbilitySpecHandle Handle, bool InputPressed, FPredictionKey* PredictionKey, FGameplayEventData EventData)
{
	AbilitySystemComponent->ServerTryActivateAbilityWithEventData(Handle, InputPressed, PredictionKey, EventData);
}

void UFortAbilitySystemComponentAthenaPatch::Init()
{
	UKismetDetoursLibrary::AddReflectedDetour<UFortAbilitySystemComponentAthena>(
		L"ServerTryActivateAbility",
		EReflectedDetourType::VFSwap,
		ServerTryActivateAbilityHook,
		_ServerTryActivateAbility,
		1
	);

	UKismetDetoursLibrary::AddReflectedDetour<UFortAbilitySystemComponentAthena>(
		L"ServerTryActivateAbilityWithEventData",
		EReflectedDetourType::VFSwap,
		ServerTryActivateAbilityWithEventDataHook,
		_ServerTryActivateAbilityWithEventData,
		1
	);
}