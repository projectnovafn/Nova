#pragma once

#include "FortWeap_BuildingToolBase.h"

class AFortWeap_EditingTool : public AFortWeap_BuildingToolBase
{
	GENERATED_UCLASS_BODY(AFortWeap_EditingTool, FortniteGame)

public:
	UPROPERTY(bool, bEditConfirmed)
	bool bEditConfirmed;

	UPROPERTY_NOTIFY(ABuildingSMActor*, EditActor)
	ABuildingSMActor* EditActor;
};