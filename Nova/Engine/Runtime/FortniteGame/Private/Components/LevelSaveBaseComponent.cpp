#include "Components/LevelSaveBaseComponent.h"

void ULevelSaveBaseComponent::SaveOutLevelRecord()
{
	this->ProcessEvent(L"SaveOutLevelRecord");
}

void ULevelSaveBaseComponent::StartPeriodicSaveTimer()
{
	this->ProcessEvent(L"StartPeriodicSaveTimer");
}

void ULevelSaveBaseComponent::StopPeriodicSaveTimer()
{
	this->ProcessEvent(L"StopPeriodicSaveTimer");
}
