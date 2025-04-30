#pragma once

#include "Engine.h"
#include "Engine/NetSerialization.h"

class FGameplayAbilityActivationInfo
{
	GENERATED_USTRUCT_BODY(FGameplayAbilityActivationInfo, GameplayAbilities)

public:

private:

	unsigned char Pad[0x20];
};

class FGameplayAbilitySpecHandle
{
	int Handle;
public:

	bool operator==(const FGameplayAbilitySpecHandle& Other) const
	{
		return Handle == Other.Handle;
	}

	void GenerateNewHandle()
	{
		static int GHandle = 1;

		Handle = GHandle++;
	}
};

class FGameplayAbilitySpec : public FFastArraySerializerItem
{
	GENERATED_USTRUCT_BODY(FGameplayAbilitySpec, GameplayAbilities)
public:

	UPROPERTY_STRUCT(FGameplayAbilitySpecHandle, Handle)
	FGameplayAbilitySpecHandle Handle;

	UPROPERTY_STRUCT(class UGameplayAbility*, Ability)
	class UGameplayAbility* Ability;

	UPROPERTY_STRUCT(int, Level)
	int Level;

	UPROPERTY_STRUCT(int, InputID)
	int InputID;

	UPROPERTY_STRUCT(UObject*, SourceObject)
	UObject* SourceObject;

	UPROPERTY_STRUCT_BOOL(InputPressed)
	bool InputPressed;

	UPROPERTY_STRUCT_BOOL(RemoveAfterActivation)
	bool RemoveAfterActivation;

	UPROPERTY_STRUCT(FGameplayAbilityActivationInfo, ActivationInfo)
	FGameplayAbilityActivationInfo ActivationInfo;
};

class FGameplayAbilitySpecContainer : public FFastArraySerializer
{
	GENERATED_USTRUCT_BODY(FGameplayAbilitySpecContainer, GameplayAbilities)

public:

	UPROPERTY_STRUCT(TArray<FGameplayAbilitySpec>, Items)
	TArray<FGameplayAbilitySpec> Items;

	UPROPERTY_STRUCT(class UAbilitySystemComponent*, Owner)
	class UAbilitySystemComponent* Owner;
};

class FGameplayEffectContextHandle
{
	GENERATED_USTRUCT_BODY(FGameplayEffectContextHandle, GameplayAbilities)

private:

	char Pad[0x18];
};

class FActiveGameplayEffectHandle
{
	GENERATED_USTRUCT_BODY(FActiveGameplayEffectHandle, GameplayAbilities)

public:

	UPROPERTY_STRUCT(int, Handle)
	int Handle;
};