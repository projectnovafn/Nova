#include "Pawns/FortPawn.h"

void AFortPawn::OnRep_IsDBNO()
{
	this->ProcessEvent(L"OnRep_IsDBNO");
}

AFortWeapon* AFortPawn::EquipWeaponDefinition(UFortItemDefinition* WeaponData, FGuid ItemEntryGuid)
{
	struct
	{
		UFortItemDefinition* WeaponData;
		FGuid ItemEntry;
		AFortWeapon* ReturnValue;
	} params(WeaponData, ItemEntryGuid);

	this->ProcessEvent(L"EquipWeaponDefinition", &params);

	return params.ReturnValue;
}

void AFortPawn::ForceKill(FGameplayTag DeathReason, AController* KillerController, AActor* KillerActor)
{
	struct
	{
		FGameplayTag DeathReason;
		AController* KillerController;
		AActor* KillerActor;
	} params(DeathReason, KillerController, KillerActor);

	this->ProcessEvent(L"ForceKill", &params);
}

void AFortPawn::LaunchCharacterJump(FVector LaunchVelocity, bool bXYOverride, bool bZOverride, bool bIgnoreFallDamage, bool bPlayFeedbackEvent)
{
	struct
	{
		FVector LaunchVelocity;
		bool bXYOverride;
		bool bZOverride; 
		bool bIgnoreFallDamage;
		bool bPlayFeedbackEvent;
	} params(LaunchVelocity, bXYOverride, bZOverride, bIgnoreFallDamage, bPlayFeedbackEvent);

	this->ProcessEvent(L"LaunchCharacterJump", &params);
}

void AFortPawn::SetHealth(float Health)
{
	this->ProcessEvent(L"SetHealth", &Health);
}

void AFortPawn::ClientInternalEquipWeapon(AFortWeapon* Weapon)
{
	this->ProcessEvent(L"ClientInternalEquipWeapon", &Weapon);
}

float AFortPawn::GetShield()
{
	float ReturnValue;
	this->ProcessEvent(L"GetShield", &ReturnValue);

	return ReturnValue;
}

float AFortPawn::GetHealth()
{
	float ReturnValue;
	this->ProcessEvent(L"GetHealth", &ReturnValue);

	return ReturnValue;
}

float AFortPawn::GetMaxShield()
{
	float ReturnValue;
	this->ProcessEvent(L"GetMaxShield", &ReturnValue);

	return ReturnValue;
}

float AFortPawn::GetMaxHealth()
{
	float ReturnValue;
	this->ProcessEvent(L"GetMaxHealth", &ReturnValue);

	return ReturnValue;
}

void AFortPawn::StopPerformingNativeAction(FGameplayTag NativeActionTag)
{
	static auto StopPerformingNativeAction = UKismetMemoryLibrary::Get<void(*)(AFortPawn*,	FGameplayTag&)>(L"AFortPawn::StopPerformingNativeAction");

	return StopPerformingNativeAction(this, NativeActionTag);
}

bool AFortPawn::PickUpActor(AActor* PickupTarget, UFortDecoItemDefinition* ItemDefinition)
{
	struct
	{
		AActor* PickupTarget;
		UFortDecoItemDefinition* ItemDefinition;
		bool ReturnValue;
	}params(PickupTarget, ItemDefinition);

	this->ProcessEvent(L"PickUpActor", &params);

	return params.ReturnValue;
}

void AFortPawn::SetShield(float NewShieldValue)
{
	this->ProcessEvent(L"SetShield", &NewShieldValue);
}

void AFortPawn::MovingEmoteStopped()
{
	if (GetClassProperty(L"bMovingEmote") != NULL)
		bMovingEmote = false;

	if (GetClassProperty(L"bMovingEmoteForwardOnly") != NULL)
		bMovingEmoteForwardOnly = false;
}