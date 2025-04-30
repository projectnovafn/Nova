#pragma once

#include "CoreUObject.h"

class UGameInstance;
enum class EHotfixResult : uint8
{
	Failed,
	Success,
	SuccessNoChange,
	SuccessNeedsReload,
	SuccessNeedsRelaunch
};

enum class EUpdateState : uint8
{
	UpdateIdle,
	UpdatePending,
	CheckingForPatch,
	DetectingPlatformEnvironment,
	CheckingForHotfix,
	WaitingOnInitialLoad,
	InitialLoadComplete,
	UpdateComplete
};

enum class EUpdateCompletionStatus : uint8
{
	UpdateUnknown,
	UpdateSuccess,
	UpdateSuccess_NoChange,
	UpdateSuccess_NeedsReload,
	UpdateSuccess_NeedsRelaunch,
	UpdateSuccess_NeedsPatch,
	UpdateFailure_PatchCheck,
	UpdateFailure_HotfixCheck,
	UpdateFailure_NotLoggedIn
};

enum class EPatchCheckResult : uint8
{
	NoPatchRequired,
	PatchRequired,
	NoLoggedInUser,
	PatchCheckFailure
};

class UUpdateManager : public UObject
{
	GENERATED_UCLASS_BODY(UUpdateManager, Hotfix)

private:

	UPROPERTY(float, HotfixCheckCompleteDelay)
	float HotfixCheckCompleteDelay;

	UPROPERTY(float, UpdateCheckCompleteDelay)
	float UpdateCheckCompleteDelay;

	UPROPERTY(float, HotfixAvailabilityCheckCompleteDelay)
	float HotfixAvailabilityCheckCompleteDelay;

	UPROPERTY(float, UpdateCheckAvailabilityCompleteDelay)
	float UpdateCheckAvailabilityCompleteDelay;

	UPROPERTY(bool, bPlatformEnvironmentDetected)
	bool bPlatformEnvironmentDetected;

	UPROPERTY_BOOL(bInitialUpdateFinished)
	bool bInitialUpdateFinished;

	UPROPERTY(bool, bCheckHotfixAvailabilityOnly)
	bool bCheckHotfixAvailabilityOnly;

	UPROPERTY(EUpdateState, CurrentUpdateState)
	EUpdateState CurrentUpdateState;

	UPROPERTY(int32, WorstNumFilesPendingLoadViewed)
	int32 WorstNumFilesPendingLoadViewed;

	UPROPERTY(EPatchCheckResult, LastPatchCheckResult)
	EPatchCheckResult LastPatchCheckResult;

	UPROPERTY(EHotfixResult, LastHotfixResult)
	EHotfixResult LastHotfixResult;

public:

	void StartPatchCheck();
	void PatchCheckComplete(EPatchCheckResult Result);
};