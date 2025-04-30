#pragma once
#include "GameFramework/Info.h"

class AController;

class AFortTeamInfo : public AInfo
{
	GENERATED_UCLASS_BODY(AFortTeamInfo, FortniteGame)

public:

	UPROPERTY(TArray<AController*>, TeamMembers)
	TArray<AController*> TeamMembers;

	bool IsAlive();
};