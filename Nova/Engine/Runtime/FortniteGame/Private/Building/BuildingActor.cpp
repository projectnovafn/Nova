#include "Building/BuildingActor.h"

void ABuildingActor::SetTeam(uint8 Team)
{
	static auto bHasTeamIndex = !!this->GetClassProperty(L"TeamIndex");

	if (bHasTeamIndex)
		this->TeamIndex = Team;
	else
		this->Team = Team;

	this->OnRep_Team();
}

uint8 ABuildingActor::GetTeam()
{
	static auto bHasTeamIndex = !!this->GetClassProperty(L"TeamIndex");

	if (bHasTeamIndex)
		return TeamIndex;
	else
		return Team;
}

void ABuildingActor::InitializeKismetSpawnedBuildingActor(ABuildingActor* BuildingOwner, AFortPlayerController* SpawningController, bool bUsePlayerBuildAnimations)
{
	struct
	{
		ABuildingActor* BuildingOwner;
		AFortPlayerController* SpawningController;
		bool bUsePlayerBuildAnimations;
	} params(BuildingOwner, SpawningController, bUsePlayerBuildAnimations);

	ProcessEvent(L"InitializeKismetSpawnedBuildingActor", &params);
}

void ABuildingActor::OnRep_Team()
{
	ProcessEvent(L"OnRep_Team");
}

float ABuildingActor::GetHealth()
{
	float Value;
	this->ProcessEvent(L"GetHealth", &Value);

	return Value;
}

float ABuildingActor::GetMaxHealth()
{
	float Value;
	this->ProcessEvent(L"GetMaxHealth", &Value);

	return Value;
}

float ABuildingActor::GetHealthPercent()
{
	float Value;
	this->ProcessEvent(L"GetHealthPercent", &Value);

	return Value;
}