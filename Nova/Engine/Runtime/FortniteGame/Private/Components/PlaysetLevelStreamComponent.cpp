#include "Components/PlaysetLevelStreamComponent.h"

void UPlaysetLevelStreamComponent::SetPlayset(UFortPlaysetItemDefinition* NewPlayset)
{
	this->ProcessEvent(L"SetPlayset", &NewPlayset);
}

void UPlaysetLevelStreamComponent::LoadPlaysetAsync()
{
	static auto LoadPlaysetAsync = UKismetMemoryLibrary::Get<void (*)(UPlaysetLevelStreamComponent*)>(L"UPlaysetLevelStreamComponent::LoadPlaysetAsync");

	return LoadPlaysetAsync(this);
}

void UPlaysetLevelStreamComponent::UnloadPlaysetAsync()
{
	static auto UnloadPlaysetAsync = UKismetMemoryLibrary::Get<void (*)(UPlaysetLevelStreamComponent*, bool)>(L"UPlaysetLevelStreamComponent::UnloadPlaysetAsync");

	return UnloadPlaysetAsync(this, true);
}