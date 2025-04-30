#pragma once
#include "FortGameSession.h"
#include "FortPlaylistManager.h"
#include "Engine/World.h"

#include "Kismet/KismetSystemLibrary.h"

class FMatchmakingDedicatedServerMatchAssignment
{
public:

	inline FName& InternalGetPlaylistName()
	{
		return *(FName*)(__int64(this) + (UKismetSystemLibrary::GetEngineVersionNum() >= 4.25 ? 0x28 : 0x20));
	}

	inline void InternalSetPlaylistName(const FName& InPlaylistName)
	{
		*(FName*)(__int64(this) + (UKismetSystemLibrary::GetEngineVersionNum() >= 4.25 ? 0x28 : 0x20)) = InPlaylistName;
	}

	__declspec(property(get = InternalGetPlaylistName, put = InternalSetPlaylistName))
	FName PlaylistName;
};

class AFortGameSessionDedicated : public AFortGameSession
{
	GENERATED_UCLASS_BODY(AFortGameSessionDedicated, FortniteGame)
public:

	void HandleMatchAssignment(FMatchmakingDedicatedServerMatchAssignment& Assignment);
	void Restart();
};