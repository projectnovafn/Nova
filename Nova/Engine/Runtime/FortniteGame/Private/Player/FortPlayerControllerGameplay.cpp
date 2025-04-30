#include "Player/FortPlayerControllerGameplay.h"

void AFortPlayerControllerGameplay::ServerCreativeSetFlightSpeedIndex(int32 Index)
{
	UE_LOG(LogFort, Log, TEXT("AFortPlayerControllerGameplay::ServerCreativeSetFlightSpeedIndex: Wanted: %i, Current: %i"), Index, FlyingModifierIndex);

	FlyingModifierIndex = Index;
}

void AFortPlayerControllerGameplay::ServerCreativeSetFlightSprint(bool bSprint)
{
	UE_LOG(LogFort, Log, TEXT("AFortPlayerControllerGameplay::ServerCreativeSetFlightSprint: Wanted: %i, Current: %i"), bSprint, bIsFlightSprinting);

	bIsFlightSprinting = bSprint;
	OnRep_IsFlightSprinting();
}

void AFortPlayerControllerGameplay::StartGhostMode(UFortWorldItemDefinition* ItemProvidingGhostMode)
{
}

void AFortPlayerControllerGameplay::EndGhostMode()
{
	if (!WorldInventory)
		return;
	
	auto GhostModeItemDef = GhostModeRepData.GhostModeItemDef;

	if (!GhostModeItemDef)
		GhostModeItemDef = LoadObject<UFortWorldItemDefinition>(L"/Game/Athena/Items/Gameplay/SpookyMist/AGID_SpookyMist.AGID_SpookyMist");

	if (auto ItemEntry = WorldInventory->FindItemEntry(GhostModeItemDef))
	{
		WorldInventory->RemoveItem(ItemEntry->ItemGuid, ItemEntry->Count);
		WorldInventory->Update();
	}

	this->ClientEquipItem(WorldInventory->GetPickaxeItem(), true);
}

void AFortPlayerControllerGameplay::ServerCreativeToggleFly()
{
	this->ProcessEvent(L"ServerCreativeToggleFly");
}

void AFortPlayerControllerGameplay::OnRep_IsFlightSprinting()
{
	ProcessEvent(L"OnRep_IsFlightSprinting");
}