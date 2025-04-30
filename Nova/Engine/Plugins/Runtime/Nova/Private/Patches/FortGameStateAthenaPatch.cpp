#include "Patches/FortGameStateAthenaPatch.h"
#include "KismetDetoursLibrary.h"

bool (*_IsResurrectionEnabled)(AFortGameStateAthena*, AFortPlayerPawn*);
TArray<TWeakObjectPtr<AFortPlayerStateAthena>>* (*_FindTeamMembers)(AFortGameStateAthena*, uint8);
TArray<TWeakObjectPtr<AFortPlayerStateAthena>>* (*_FindSquadMembers)(AFortGameStateAthena*, uint8);

UDetour* FindTeamMembersDetour = NULL;
UDetour* FindSquadMembersDetour = NULL;

bool AFortGameStateAthenaPatch::IsResurrectionEnabledHook(AFortGameStateAthena* GameState, AFortPlayerPawn* Pawn)
{
	return GameState->IsResurrectionEnabled(Pawn);
}

TArray<TWeakObjectPtr<AFortPlayerStateAthena>>* AFortGameStateAthenaPatch::FindTeamMembersHook(AFortGameStateAthena* GameState, uint8 TeamIndex)
{
	return GameState->FindTeamMembers(TeamIndex);
}

TArray<TWeakObjectPtr<AFortPlayerStateAthena>>* AFortGameStateAthenaPatch::FindSquadMembersHook(AFortGameStateAthena* GameState, uint8 SquadId)
{
	return GameState->FindSquadMembers(SquadId);
}

void AFortGameStateAthenaPatch::Init()
{
	_FindTeamMembers = UKismetMemoryLibrary::Get<decltype(_FindTeamMembers)>(L"AFortGameStateAthenaPatch::FindTeamMembers");
	_FindSquadMembers = UKismetMemoryLibrary::Get<decltype(_FindSquadMembers)>(L"AFortGameStateAthenaPatch::FindSquadMembers");

	FindTeamMembersDetour = new UDetour();
	FindTeamMembersDetour->Init(_FindTeamMembers, FindTeamMembersHook);
	FindTeamMembersDetour->Commit();

	FindSquadMembersDetour = new UDetour();
	FindSquadMembersDetour->Init(_FindSquadMembers, FindSquadMembersHook);
	FindSquadMembersDetour->Commit();

	UKismetDetoursLibrary::AddReflectedDetour<AFortGameStateAthena>(
		L"IsResurrectionEnabled",
		EReflectedDetourType::ImplDetour,
		IsResurrectionEnabledHook,
		_IsResurrectionEnabled,
		0,
		{ 0x48, 0x03 }
	);
}