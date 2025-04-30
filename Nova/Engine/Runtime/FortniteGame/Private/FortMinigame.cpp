#include "FortMinigame.h"

#include "Athena/Player/FortPlayerControllerAthena.h"
#include "Player/FortPlayerState.h"

void AFortMinigame::SetState(EFortMinigameState State)
{
	UE_LOG(LogFort, Log, TEXT("AFortMinigame::SetState: %i"), State);

	if (State == EFortMinigameState::Transitioning)
	{
		QueueTask(ENamedThreads::GameThread, 3.0f, [this]
		{
			CurrentState = EFortMinigameState::Warmup;
		});
	}
}

void AFortMinigame::NativeExitedVolumeEvent(AFortPlayerState* LeftPlayerState)
{
	UE_LOG(LogFort, Log, TEXT("NativeExitedVolumeEvent: %s"), *LeftPlayerState->GetName());

	auto PlayerController = Cast<AFortPlayerControllerAthena>(LeftPlayerState->GetOwner());

	if (!PlayerController)
		return;

	auto CreativeMoveTool = PlayerController->CreativeMoveToolItemDefinition.Get();
	if (!CreativeMoveTool)
		return;

	if (auto ItemEntry = PlayerController->WorldInventory->FindItemEntry(CreativeMoveTool))
	{
		PlayerController->WorldInventory->RemoveItem(ItemEntry->ItemGuid, ItemEntry->Count);
		PlayerController->WorldInventory->Update();

		PlayerController->ClientEquipItem(PlayerController->WorldInventory->GetPickaxeItem(), true);
	}

	PlayerController->WorldInventory->Clear();
}

void AFortMinigame::NativeEnteredVolumeEvent(AFortPlayerState* PlayerState)
{
	UE_LOG(LogFort, Log, TEXT("NativeEnteredVolumeEvent: %s"), *PlayerState->GetName());

	auto PlayerController = Cast<AFortPlayerControllerAthena>(PlayerState->GetOwner());

	if (!PlayerController)
		return;

	auto CreativeMoveTool = PlayerController->CreativeMoveToolItemDefinition.Get();

	if (!CreativeMoveTool)
		return;

	if (PlayerController->WorldInventory->FindItemEntry(CreativeMoveTool))
		return;

	PlayerController->WorldInventory->AddItem(CreativeMoveTool, 1);
	PlayerController->WorldInventory->Update();
}

void AFortMinigame::ClearPlayerInventory(AFortPlayerControllerAthena* PlayerController)
{
	UE_LOG(LogFort, Log, TEXT("ClearPlayerInventory"));

	PlayerController->WorldInventory->Clear();
}
