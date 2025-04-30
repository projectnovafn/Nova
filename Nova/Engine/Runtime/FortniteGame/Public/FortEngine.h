#pragma once

#include "Engine.h"
#include "EngineSettings.h"

class UFortEngine : public UGameEngine
{
	GENERATED_UCLASS_BODY(UFortEngine, FortniteGame)

public:

	void Init(void* EngineLoop);

	float GetMaxTickRate(float DeltaTime, bool bAllowFrameSmoothing);
};