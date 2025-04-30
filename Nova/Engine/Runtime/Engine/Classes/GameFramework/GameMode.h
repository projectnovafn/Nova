#pragma once
#include "GameModeBase.h"

class AGameMode : public AGameModeBase
{
	GENERATED_UCLASS_BODY(AGameMode, Engine)

public:

	void RestartGame();
};