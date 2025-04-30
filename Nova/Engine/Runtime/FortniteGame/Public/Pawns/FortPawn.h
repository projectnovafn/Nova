#pragma once

#include "GameFramework/Character.h"
#include "Weapons/FortWeapon.h"
#include "GameplayTagContainer.h"
#include "AbilitySystemComponent.h"

class UFortDecoItemDefinition;

class AFortPawn : public ACharacter
{
	GENERATED_UCLASS_BODY(AFortPawn, FortniteGame)
public:

	UPROPERTY(UAbilitySystemComponent*, AbilitySystemComponent)
	UAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY_NOTIFY(AFortWeapon*, CurrentWeapon)
	AFortWeapon* CurrentWeapon;

	UPROPERTY(TArray<AFortWeapon*>, CurrentWeaponList)
	TArray<AFortWeapon*> CurrentWeaponList;

	UPROPERTY_BOOL(bIsDBNO)
	bool bIsDBNO;

	UPROPERTY_BOOL(bIsDying)
	bool bIsDying;

	UPROPERTY_BOOL(bPlayedDying)
	bool bPlayedDying;

	UPROPERTY(int, DBNORevivalStacking)
	int DBNORevivalStacking;

	UPROPERTY_BOOL(bMovingEmote)
	bool bMovingEmote;

	UPROPERTY_BOOL(bMovingEmoteForwardOnly)
	bool bMovingEmoteForwardOnly;

	UPROPERTY(float, EmoteWalkSpeed)
	float EmoteWalkSpeed;

	UPROPERTY(FGameplayTagContainer, DeathTags)
	FGameplayTagContainer DeathTags;

	void OnRep_IsDBNO();

	AFortWeapon* EquipWeaponDefinition(UFortItemDefinition* WeaponData, FGuid ItemEntryGuid);

	void ForceKill(FGameplayTag DeathReason, AController* KillerController, AActor* KillerActor);

	void LaunchCharacterJump(FVector LaunchVelocity, bool bXYOverride, bool bZOverride, bool bIgnoreFallDamage, bool bPlayFeedbackEvent);

	void StopPerformingNativeAction(FGameplayTag NativeActionTag);

	bool PickUpActor(AActor* PickupTarget, UFortDecoItemDefinition* ItemDefinition);

	void SetShield(float NewShieldValue);
	void SetHealth(float Health);

	void ClientInternalEquipWeapon(AFortWeapon* Weapon);

	float GetShield();
	float GetHealth();

	float GetMaxShield();
	float GetMaxHealth();

	void MovingEmoteStopped();
};