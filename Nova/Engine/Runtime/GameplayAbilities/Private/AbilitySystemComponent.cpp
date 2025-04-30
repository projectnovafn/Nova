#include "AbilitySystemComponent.h"

#include "Kismet/KismetSystemLibrary.h"

#include <vendor/memcury.h>

FGameplayAbilitySpec* UAbilitySystemComponent::FindAbilitySpecFromHandle(FGameplayAbilitySpecHandle Handle)
{
	for (auto& Item : ActivatableAbilities.Items)
	{
		if (Item.Handle == Handle)
			return &Item;
	}

	return nullptr;
}

FGameplayAbilitySpec* UAbilitySystemComponent::FindAbilitySpecFromClass(UClass* AbilityClass)
{
	for (auto& Item : ActivatableAbilities.Items)
	{
		if (Item.Ability->GetClass() == AbilityClass)
			return &Item;
	}

	return nullptr;
}

FActiveGameplayEffectHandle UAbilitySystemComponent::ApplyGameplayEffectToSelf(UClass* GameplayEffectClass, float Level, FGameplayEffectContextHandle EffectContext)
{
	struct
	{
		UClass* GameplayEffectClass;
		float Level;
		FGameplayEffectContextHandle EffectContext;
		FActiveGameplayEffectHandle ReturnValue;
	} params(GameplayEffectClass, Level, EffectContext);

	this->ProcessEvent(L"BP_ApplyGameplayEffectToSelf", &params);

	return params.ReturnValue;
}

void UAbilitySystemComponent::InvokeGameplayCueExecuted(FGameplayTag GameplayCueTag, FPredictionKey PredictionKey, FGameplayEffectContextHandle EffectContext)
{
	struct
	{
		FGameplayTag GameplayCueTag; 
		FPredictionKey PredictionKey; 
		FGameplayEffectContextHandle EffectContext;
	} params(GameplayCueTag, PredictionKey, EffectContext);

	this->ProcessEvent(L"NetMulticast_InvokeGameplayCueExecuted", &params);
}

void UAbilitySystemComponent::ClientActivateAbilityFailed(FGameplayAbilitySpecHandle Handle, uint16 PredictionKey)
{
	struct
	{
		FGameplayAbilitySpecHandle Handle;
		uint16 PredictionKey;
	} params(Handle, PredictionKey);

	ProcessEvent(L"ClientActivateAbilityFailed", &params);
}

FGameplayAbilitySpecHandle UAbilitySystemComponent::GiveAbility(UClass* AbilityClass, UObject* SourceObject)
{
	static auto FGameplayAbilitySpecCtor = UKismetMemoryLibrary::Get<void (*)(FGameplayAbilitySpec*, UGameplayAbility*, int32, int32, UObject*)>(L"FGameplayAbilitySpecCtor");
	static auto GiveAbility = UKismetMemoryLibrary::Get<void (*)(UAbilitySystemComponent*, FGameplayAbilitySpecHandle&, FGameplayAbilitySpec*)>(L"UAbilitySystemComponent::GiveAbility");

	auto Spec = FGameplayAbilitySpec::Allocate();

	FGameplayAbilitySpecCtor(Spec, AbilityClass->GetDefaultObject<UGameplayAbility>(), 1, -1, NULL);

	Spec->SourceObject = SourceObject;

	GiveAbility(this, Spec->Handle, Spec);

	return Spec->Handle;
}

void UAbilitySystemComponent::GiveAbilityAndActivateOnce(UClass* AbilityClass, UObject* SourceObject, FGameplayEventData* EventData)
{
	static auto FGameplayAbilitySpecCtor = UKismetMemoryLibrary::Get<void (*)(FGameplayAbilitySpec*, UGameplayAbility*, int32, int32, UObject*)>(L"FGameplayAbilitySpecCtor");
	static auto GiveAbility = UKismetMemoryLibrary::Get<void (*)(UAbilitySystemComponent*, FGameplayAbilitySpecHandle&, FGameplayAbilitySpec*)>(L"UAbilitySystemComponent::GiveAbility");

	auto Spec = FGameplayAbilitySpec::Allocate();

	FGameplayAbilitySpecCtor(Spec, AbilityClass->GetDefaultObject<UGameplayAbility>(), 1, -1, NULL);

	Spec->RemoveAfterActivation = true;
	Spec->SourceObject = SourceObject;

	GiveAbility(this, Spec->Handle, Spec);

	auto PredictionKey = FPredictionKey::Allocate();
	UObject* InstancedAbility = NULL;

	InternalTryActivateAbility(Spec->Handle, PredictionKey, &InstancedAbility, nullptr, EventData);
}

bool UAbilitySystemComponent::InternalTryActivateAbility(FGameplayAbilitySpecHandle Handle, FPredictionKey* InPredictionKey, UObject** OutInstancedAbility, void* OnGameplayAbilityEndedDelegate, void* TriggerEventData)
{
	static auto InternalTryActivateAbility = UKismetMemoryLibrary::Get<bool (*)(UAbilitySystemComponent*, FGameplayAbilitySpecHandle, FPredictionKey, UObject**, void*, void*)>(L"UAbilitySystemComponent::InternalTryActivateAbility");

	return InternalTryActivateAbility(this, Handle, *InPredictionKey, OutInstancedAbility, OnGameplayAbilityEndedDelegate, TriggerEventData);
}

void UAbilitySystemComponent::ServerTryActivateAbility(FGameplayAbilitySpecHandle Handle, bool InputPressed, FPredictionKey* PredictionKey)
{
	auto Spec = FindAbilitySpecFromHandle(Handle);

	if (!Spec)
		return ClientActivateAbilityFailed(Handle, PredictionKey->Current);

	ConsumeAllReplicatedData(Handle, PredictionKey);

	Spec->InputPressed = true;

	UObject* InstancedAbility;
	if (!InternalTryActivateAbility(Handle, PredictionKey, &InstancedAbility, nullptr, nullptr))
	{
		Spec->InputPressed = false;

		ClientActivateAbilityFailed(Handle, PredictionKey->Current);
		return ActivatableAbilities.MarkItemDirty(*Spec);
	}
}

void UAbilitySystemComponent::ServerTryActivateAbilityWithEventData(FGameplayAbilitySpecHandle Handle, bool InputPressed, FPredictionKey* PredictionKey, FGameplayEventData EventData)
{
	auto Spec = FindAbilitySpecFromHandle(Handle);

	if (!Spec)
		return ClientActivateAbilityFailed(Handle, PredictionKey->Current);

	ConsumeAllReplicatedData(Handle, PredictionKey);

	Spec->InputPressed = true;

	UObject* InstancedAbility;
	if (!InternalTryActivateAbility(Handle, PredictionKey, &InstancedAbility, nullptr, &EventData))
	{
		Spec->InputPressed = false;

		ClientActivateAbilityFailed(Handle, PredictionKey->Current);
		return ActivatableAbilities.MarkItemDirty(*Spec);
	}
}

void UAbilitySystemComponent::ConsumeAllReplicatedData(FGameplayAbilitySpecHandle AbilityHandle, FPredictionKey* AbilityOriginalPredictionKey)
{
	static auto ServerSetReplicatedTargetDataCancelled = UKismetMemoryLibrary::Get<void (*)(UAbilitySystemComponent*, FGameplayAbilitySpecHandle, FPredictionKey, FPredictionKey)>(L"UAbilitySystemComponent::ServerSetReplicatedTargetDataCancelled");
	if (ServerSetReplicatedTargetDataCancelled && UKismetSystemLibrary::GetEngineVersionNum() <= 4.21)
	{
		static auto PatchStart = Memcury::Scanner(ServerSetReplicatedTargetDataCancelled)
			.ScanFor({ 0x48, 0x8B, 0xCB })
			.ScanFor({ 0xC6 })
			.GetAs<uint8*>();
		static auto PatchEnd = Memcury::Scanner(PatchStart)
			.ScanFor({ 0xE8 })
			.AbsoluteOffset(5)
			.GetAs<uint8*>();
		static auto PatchSize = PatchEnd - PatchStart;

		uint8_t RestoreBytes[1024];

		FMemory::Memcpy(RestoreBytes, PatchStart, PatchSize);
		FMemory::Memset(PatchStart, 0x90, PatchSize);

		ServerSetReplicatedTargetDataCancelled(this, AbilityHandle, *AbilityOriginalPredictionKey, {});

		FMemory::Memcpy(PatchStart, RestoreBytes, PatchSize);
	}
}

void UAbilitySystemComponent::CancelAbilities(FGameplayTagContainer* WithTags, FGameplayTagContainer* WithoutTags, UGameplayAbility* Ignore)
{
	static auto CancelAbilities = UKismetMemoryLibrary::Get<void(*)(UAbilitySystemComponent*, FGameplayTagContainer*, FGameplayTagContainer*, UGameplayAbility*)>(L"UAbilitySystemComponent::CancelAbilities");
	return CancelAbilities(this, WithTags, WithoutTags, Ignore);
}
