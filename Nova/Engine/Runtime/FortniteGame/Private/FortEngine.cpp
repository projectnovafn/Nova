#include "FortEngine.h"
#include "FortGlobals.h"
#include "Nova.h"

void UFortEngine::Init(void* EngineLoop)
{
	GEngine = this;

	GNova->OnEngineInit();

	if (!GNova->UseLateInitialization())
	{
		*GIsServer = true;
		*GIsClient = false;

		if (auto MapSettings = GetDefaultObject<UGameMapsSettings>())
		{
			if (auto FortGlobals = GetDefaultObject<UFortGlobals>())
			{
				MapSettings->GameDefaultMap.SetPath(*FortGlobals->GetBRMapName());
			}
		}
	}
}

float UFortEngine::GetMaxTickRate(float DeltaTime, bool bAllowFrameSmoothing)
{
	if (GNova->bIsLateGame)
		return 120.0f;

	return 30.0f;
}