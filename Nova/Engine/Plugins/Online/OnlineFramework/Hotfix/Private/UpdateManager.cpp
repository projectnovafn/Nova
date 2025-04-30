#include "UpdateManager.h"
#include "Engine.h"
#include "Online/FortGameSessionDedicated.h"

void UUpdateManager::StartPatchCheck()
{
	UE_LOG(LogTemp, Log, TEXT("StartPatchCheck"));

	PatchCheckComplete(EPatchCheckResult::NoPatchRequired);
}

void UUpdateManager::PatchCheckComplete(EPatchCheckResult Result)
{
	static auto PatchCheckComplete = UKismetMemoryLibrary::Get<void (*)(UUpdateManager*, EPatchCheckResult)>(L"UUpdateManager::PatchCheckComplete");

	PatchCheckComplete(this, Result);
}