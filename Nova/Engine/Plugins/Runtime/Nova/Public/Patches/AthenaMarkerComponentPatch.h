#pragma once

#include "Components/AthenaMarkerComponent.h"

class UAthenaMarkerComponentPatch
{
private:

	static void ServerRemoveMapMarker(UAthenaMarkerComponent*, FMarkerID, ECancelMarkerReason);
	static void ServerAddMapMarker(UAthenaMarkerComponent*, FFortClientMarkerRequest&);

public:

	static void Init();
};