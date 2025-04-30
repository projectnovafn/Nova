#include "Components/AthenaResurrectionComponent.h"

void UAthenaResurrectionComponent::OnRep_PlayerIdsForResurrection()
{
	this->ProcessEvent(L"OnRep_PlayerIdsForResurrection");
}