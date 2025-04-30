#pragma once
#include "Engine.h"
#include "Components/ActorComponent.h"

#include "GameplayAbility.h"
#include "GameplayAbilitySpec.h"
#include "GameplayPrediction.h"
#include "GameplayAbilityTypes.h"
#include "GameplayTagContainer.h"

class UAbilitySystemComponent : public UActorComponent
{
	GENERATED_UCLASS_BODY(UAbilitySystemComponent, GameplayAbilities)
public:

	UPROPERTY(FGameplayAbilitySpecContainer, ActivatableAbilities)
	FGameplayAbilitySpecContainer ActivatableAbilities;

	TArray<void*>& InternalGetGameplayEventTagContainerDelegates() {
		static auto Offset = this->GetClassPropertyOffset(L"AllReplicatedInstancedAbilities") - 0x10; 
		return *(TArray<void*>*)(int64(this) + Offset);
	}
	__declspec(property(get = InternalGetGameplayEventTagContainerDelegates))
	TArray<void*> GameplayEventTagContainerDelegates;

	FActiveGameplayEffectHandle ApplyGameplayEffectToSelf(UClass* GameplayEffectClass, float Level, FGameplayEffectContextHandle EffectContext);
	void InvokeGameplayCueExecuted(FGameplayTag GameplayCueTag, FPredictionKey PredictionKey, FGameplayEffectContextHandle EffectContext);

	void ClientActivateAbilityFailed(FGameplayAbilitySpecHandle Handle, uint16 PredictionKey);

	FGameplayAbilitySpec* FindAbilitySpecFromHandle(FGameplayAbilitySpecHandle Handle);
	FGameplayAbilitySpec* FindAbilitySpecFromClass(UClass* AbilityClass);

	FGameplayAbilitySpecHandle GiveAbility(UClass* AbilityClass, UObject* SourceObject = NULL);
	void GiveAbilityAndActivateOnce(UClass* AbilityClass, UObject* SourceObject = NULL, FGameplayEventData* EventData = NULL);

	bool InternalTryActivateAbility(FGameplayAbilitySpecHandle Handle, FPredictionKey* InPredictionKey, UObject** OutInstancedAbility, void* OnGameplayAbilityEndedDelegate, void* TriggerEventData);

	void ServerTryActivateAbility(FGameplayAbilitySpecHandle Handle, bool InputPressed, FPredictionKey* PredictionKey);
	void ServerTryActivateAbilityWithEventData(FGameplayAbilitySpecHandle Handle, bool InputPressed, FPredictionKey* PredictionKey, FGameplayEventData EventData);
	
	void ConsumeAllReplicatedData(FGameplayAbilitySpecHandle AbilityHandle, FPredictionKey* AbilityOriginalPredictionKey);

	void CancelAbilities(FGameplayTagContainer* WithTags, FGameplayTagContainer* WithoutTags, UGameplayAbility* Ignore);
};