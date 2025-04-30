#include "Components/FortLevelSaveComponent.h"
#include <vendor/memcury.h>

UObject* UFortLevelSaveComponent::GetSidecar()
{
	UE_LOG(LogFort, Log, TEXT("UFortLevelSaveComponent::GetSidecar"));
	return nullptr;
}

void UFortLevelSaveComponent::ResetLevelRecord()
{
	static auto ResetLevelRecord = UKismetMemoryLibrary::Get<void (*)(UFortLevelSaveComponent*)>(L"UFortLevelSaveComponent::ResetLevelRecord");

	ResetLevelRecord(this);
}

void UFortLevelSaveComponent::LoadFromDssAsync(bool bCheckoutRequired)
{
    uint8 RestoreBytes[5];

    static auto RequestExitPatch = Memcury::Scanner::FindPattern("E8 ? ? ? ? 90 E9 ? ? ? ? 8B 85 ? ? ? ? 85 C0 0F 8E ? ? ? ? E9 ? ? ? ? E9 ? ? ? ? 90")
        .GetAs<uint8*>();

    if (RequestExitPatch)
    {
        FMemory::Memcpy(RestoreBytes, RequestExitPatch, 5);
        FMemory::Memset(RequestExitPatch, 0x90, 5);
    }

    static auto LoadFromDssAsync = UKismetMemoryLibrary::Get<void* (*)(UFortLevelSaveComponent*, bool)>(L"UFortLevelSaveComponent::LoadFromDssAsync");

    LoadFromDssAsync(this, bCheckoutRequired);

    if (RequestExitPatch)
        FMemory::Memcpy(RequestExitPatch, RestoreBytes, 5);
}

void* UFortLevelSaveComponent::LoadPlaysetAsync(TSoftObjectPtr<UFortPlaysetItemDefinition> SoftPlaysetDefinition)
{
	static auto LoadPlaysetAsync = UKismetMemoryLibrary::Get<void* (*)(UFortLevelSaveComponent*, TSoftObjectPtr<UFortPlaysetItemDefinition>)>(L"UFortLevelSaveComponent::LoadPlaysetAsync");

	return LoadPlaysetAsync(this, SoftPlaysetDefinition);
}
