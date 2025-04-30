#include "Pawns/FortPlayerPawnAthena.h"
#include "Athena/Player/FortPlayerControllerAthena.h"
#include "Athena/Player/FortPlayerStateAthena.h"

#include "ItemDefinitions/FortAmmoItemDefinition.h"
#include "ItemDefinitions/FortTrapItemDefinition.h"
#include "ItemDefinitions/FortResourceItemDefinition.h"

#include "AbilitySystemComponent.h"

#include "Kismet/KismetArrayLibrary.h"

void AFortPlayerPawnAthena::ServerSendZiplineState(FZiplinePawnState& InZiplineState)
{
	UE_LOG(LogFort, Log, TEXT("ServerSendZiplineState"));

	if (InZiplineState.AuthoritativeValue > ZiplineState.AuthoritativeValue)
	{
		InZiplineState.CopyTo(&ZiplineState);

		if (!ZiplineState.bIsZiplining)
		{
			if (ZiplineState.bJumped)
			{
				float ZiplineJumpDampening = 1500;
				float ZiplineJumpStrength = 0;

				FVector LaunchVelocity{};

				LaunchVelocity.X = ZiplineJumpStrength;
				LaunchVelocity.Z = ZiplineJumpDampening;

				LaunchCharacterJump(LaunchVelocity, false, false, false, true);
			}
		}
	}

	HandleZiplineStateChanged();
}

void AFortPlayerPawnAthena::ServerReviveFromDBNO(AController* EventInstigator)
{
	static auto DBNO_ActionTag = FGameplayTag{ L"Gameplay.Action.Player.DBNO" };
	static auto DBNOAthena_ActionTag = FGameplayTag{ L"Gameplay.Action.Player.DBNOAthena" };

	static auto GAB_AthenaDBNOClass = LoadObject<UClass>(TEXT("/Game/Abilities/NPC/Generic/GAB_AthenaDBNO.GAB_AthenaDBNO_C"));
	static auto GAB_AthenaDBNOReviveClass = LoadObject<UClass>(TEXT("/Game/Abilities/NPC/Generic/GAB_AthenaDBNORevive.GAB_AthenaDBNORevive_C"));

	if (Controller == EventInstigator || !EventInstigator)
		return;

	auto FortPlayerState = Cast<AFortPlayerStateAthena>(PlayerState);
	auto FortPlayerController = Cast<AFortPlayerControllerAthena>(Controller);

	if (!FortPlayerState || !FortPlayerController)
		return;

	bIsDBNO = false;
	DBNORevivalStacking = 0;
	bIsDying = false;
	bPlayedDying = false;

	FMemory::Memzero(&FortPlayerState->DeathInfo, FDeathInfo::StaticStruct()->PropertiesSize);
	FortPlayerState->OnRep_DeathInfo();

	OnRep_IsDBNO();

	// StopPerformingNativeAction(DBNO_ActionTag);
	// StopPerformingNativeAction(DBNOAthena_ActionTag);

	SetHealth(30);

	if (auto AbilitySystemComponent = FortPlayerState->AbilitySystemComponent)
	{
		FGameplayTagContainer Tags{};

		UKismetArrayLibrary::Array_Add<FGameplayTag>(Tags.GameplayTags, FGameplayTagContainer::StaticStruct()->GetProperty(L"GameplayTags"), DBNO_ActionTag);
		UKismetArrayLibrary::Array_Add<FGameplayTag>(Tags.GameplayTags, FGameplayTagContainer::StaticStruct()->GetProperty(L"GameplayTags"), DBNOAthena_ActionTag);

		AbilitySystemComponent->CancelAbilities(&Tags, NULL, NULL);
		AbilitySystemComponent->GiveAbilityAndActivateOnce(GAB_AthenaDBNOReviveClass, NULL, new FGameplayEventData{});
	}

	FortPlayerController->ClientOnPawnRevived(EventInstigator);
}

void AFortPlayerPawnAthena::ServerChoosePart(EFortCustomPartType Part, UCustomCharacterPart* ChosenCharacterPart)
{
	UE_LOG(LogFort, Log, TEXT("ServerChoosePart: %d, %s"), (int)Part, *ChosenCharacterPart->GetName());
}

void AFortPlayerPawnAthena::TeleportToSkyDive(float HeightAboveGround)
{
	this->ProcessEvent(L"TeleportToSkyDive", &HeightAboveGround);
}

void AFortPlayerPawnAthena::ReceiveActorBeginOverlap(AActor* OtherActor)
{
	auto Pickup = Cast<AFortPickup>(OtherActor);

	if (!Pickup)
		return;

	auto ItemDef = Pickup->ItemEntry.ItemDefinition;

	if (!ItemDef)
		return;

	if (!ItemDef->IsA<UFortAmmoItemDefinition>() && !ItemDef->IsA<UFortTrapItemDefinition>() && !ItemDef->IsA<UFortResourceItemDefinition>())
		return;

	if (Pickup->PawnWhoDroppedPickup == this)
		return;

	auto PlayerController = Cast<AFortPlayerControllerAthena>(Controller);

	if (!PlayerController)
		return;

	auto WorldInventory = PlayerController->WorldInventory;

	if (!WorldInventory)
		return;

	auto ItemEntry = WorldInventory->FindItemEntry(ItemDef);

	if (!ItemEntry || (ItemEntry && ItemEntry->Count != ItemDef->MaxStackSize))
	{
		ServerHandlePickup(Pickup, 0, {}, false);
	}
}

void AFortPlayerPawnAthena::NetMulticast_Athena_BatchedDamageCues()
{
	auto PlayerController = Cast<AFortPlayerControllerAthena>(Controller);

	if (!PlayerController || !PlayerController->WorldInventory || !PlayerController->WorldInventory || !CurrentWeapon)
		return;

	PlayerController->WorldInventory->UpdateItem(CurrentWeapon->ItemEntryGuid, -1, CurrentWeapon->AmmoCount, CurrentWeapon);
}

void AFortPlayerPawnAthena::HandleZiplineStateChanged()
{
	static auto HandleZiplineStateChanged = UKismetMemoryLibrary::Get<void (*)(AFortPlayerPawnAthena*)>(L"AFortPlayerPawnAthena::HandleZiplineStateChanged");

	HandleZiplineStateChanged(this);
}