#include "Patches/AthenaMarkerComponentPatch.h"

#include "KismetDetoursLibrary.h"

#include "Kismet/KismetSystemLibrary.h"

static void (*_ServerRemoveMapMarker)(UAthenaMarkerComponent*, FMarkerID&, ECancelMarkerReason);
static void (*_ServerAddMapMarker)(UAthenaMarkerComponent*, FFortClientMarkerRequest&);

void UAthenaMarkerComponentPatch::ServerRemoveMapMarker(UAthenaMarkerComponent* MarkerComponent, FMarkerID MarkerID, ECancelMarkerReason CancelReason)
{
	MarkerComponent->ServerRemoveMapMarker(MarkerID, CancelReason);
}

void UAthenaMarkerComponentPatch::ServerAddMapMarker(UAthenaMarkerComponent* MarkerComponent, FFortClientMarkerRequest& MarkerRequest)
{
	MarkerComponent->ServerAddMapMarker(MarkerRequest);
}

void UAthenaMarkerComponentPatch::Init()
{
	if (UKismetSystemLibrary::GetEngineVersionNum() >= 4.25)
		return;

	UKismetDetoursLibrary::AddReflectedDetour<UAthenaMarkerComponent>(
		L"ServerRemoveMapMarker",
		EReflectedDetourType::VFSwap,
		ServerRemoveMapMarker,
		_ServerRemoveMapMarker,
		1
	);

	UKismetDetoursLibrary::AddReflectedDetour<UAthenaMarkerComponent>(
		L"ServerAddMapMarker",
		EReflectedDetourType::VFSwap,
		ServerAddMapMarker,
		_ServerAddMapMarker,
		1
	);
}
