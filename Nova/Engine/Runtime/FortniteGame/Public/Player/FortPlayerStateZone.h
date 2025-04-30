#pragma once
#include "FortPlayerState.h"

class AFortPlayerStateZone : public AFortPlayerState
{
	GENERATED_UCLASS_BODY(AFortPlayerStateZone, FortniteGame)
public:

	UPROPERTY(int, OldTotalScoreStat)
	int OldTotalScoreStat;
};