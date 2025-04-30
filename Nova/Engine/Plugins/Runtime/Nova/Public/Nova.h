#pragma once

#include "KismetDetoursLibrary.h"
#include "KismetMemoryLibrary.h"

class AFortPlayerControllerAthena;

class UEngine;

class UNova
{
private:

	UDetour* ExceptionFilterDetour = NULL;

public:	
	bool bIsLateGame = false;
	bool bHasSetupBus = false;
	bool bHasStartedZone = false;
	bool bHasSetZonePhase = false;

	bool UseLateInitialization();

	void Init();
	void Loaded();

	void OnEngineInit();
	void InitGlobals();
};