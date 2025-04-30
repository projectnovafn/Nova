#pragma once
#include "Info.h"

class AGameSession : public AInfo
{
	GENERATED_UCLASS_BODY(AGameSession, Engine)
public:

	UPROPERTY(int, MaxPlayers)
	int MaxPlayers;
};