#include "Patches/BuildingGameplayActorSpawnMachinePatch.h"
#include "KismetDetoursLibrary.h"

static void (*_ResurrectPlayer)(ABuildingGameplayActorSpawnMachine*);

UDetour* ResurrectPlayerDetour;

void ABuildingGameplayActorSpawnMachinePatch::ResurrectPlayerHook(ABuildingGameplayActorSpawnMachine* SpawnMachine)
{
	SpawnMachine->ResurrectPlayer();
}

void ABuildingGameplayActorSpawnMachinePatch::Init()
{
	_ResurrectPlayer = UKismetMemoryLibrary::Get<decltype(_ResurrectPlayer)>(L"ABuildingGameplayActorSpawnMachine::ResurrectPlayer");

	if (!_ResurrectPlayer)
		return;

	ResurrectPlayerDetour = new UDetour();
	ResurrectPlayerDetour->Init(_ResurrectPlayer, ResurrectPlayerHook, EDetourType::Rel32Call);
	ResurrectPlayerDetour->Commit();
}