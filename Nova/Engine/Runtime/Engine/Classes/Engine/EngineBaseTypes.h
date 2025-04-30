#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "UObject/Object.h"
#include "UObject/Class.h"
#include "Async/TaskGraphInterfaces.h"

struct FURL
{
	FString Protocol;
	FString Host;

	int32 Port;

	FString Map;
	FString RedirectURL;

	TArray<FString> Op;

	FString Portal;
	int32 Valid;
};

enum ENetMode : int64
{
	NM_Standalone = 0,
	NM_DedicatedServer = 1,
	NM_ListenServer = 2,
	NM_Client = 3,
	NM_MAX = 4
};

enum ELevelTick
{
	LEVELTICK_TimeOnly = 0,
	LEVELTICK_ViewportsOnly = 1,
	LEVELTICK_All = 2,
	LEVELTICK_PauseTick = 3
};