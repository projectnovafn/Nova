#include "Components/AthenaMarkerComponent.h"

#include "Athena/Player/FortPlayerControllerAthena.h"
#include "Athena/Player/FortPlayerStateAthena.h"

#include "Inventory/FortPickup.h"

#include "Kismet/KismetTextLibrary.h"

#include "UObject/WeakObjectPtr.h"

void UAthenaMarkerComponent::ServerRemoveMapMarker(FMarkerID MarkerID, ECancelMarkerReason CancelReason)
{
	auto PlayerController = Cast<AFortPlayerControllerAthena>(GetOwner());
	auto PlayerState = Cast<AFortPlayerStateAthena>(PlayerController->PlayerState);

	for (auto TeamMember : PlayerState->PlayerTeam->TeamMembers)
	{
		if (TeamMember == PlayerController)
			continue;

		if (auto PlayerController = Cast<AFortPlayerControllerAthena>(TeamMember))
		{
			if (PlayerController->MarkerComponent)
				PlayerController->MarkerComponent->ClientCancelMarker(MarkerID);
		}
	}
}

void UAthenaMarkerComponent::ServerAddMapMarker(FFortClientMarkerRequest& MarkerRequest)
{
	auto PlayerController = Cast<AFortPlayerControllerAthena>(GetOwner());
	auto PlayerState = Cast<AFortPlayerStateAthena>(PlayerController->PlayerState);

	auto MarkerData = FFortWorldMarkerData::Allocate();

	MarkerData->Owner = PlayerState;

	MarkerData->MarkerID = FMarkerID{};
	MarkerData->MarkerID.InstanceID = MarkerRequest.InstanceID;
	MarkerData->MarkerID.PlayerID = MarkerData->Owner->PlayerID;

	MarkerData->MarkerType = MarkerRequest.MarkerType;

	MarkerData->WorldPosition = MarkerRequest.WorldPosition;
	MarkerData->WorldPositionOffset = MarkerRequest.WorldPositionOffset;
	MarkerData->WorldNormal = MarkerRequest.WorldNormal;

	MarkerData->CustomDisplayInfo = *FMarkedActorDisplayInfo::Allocate();
	MarkerData->CustomDisplayInfo.DisplayName = UKismetTextLibrary::Conv_StringToText(L"");

	MarkerData->CustomDisplayInfo.Icon.SoftObjectPtr.WeakPtr = FWeakObjectPtr(nullptr);

	if (auto Pickup = Cast<AFortPickup>(MarkerRequest.MarkedActor))
	{
		MarkerData->ItemDefinition = Pickup->ItemEntry.ItemDefinition;
		MarkerData->ItemCount = Pickup->ItemEntry.Count;
	}

	if (MarkerRequest.MarkedActor)
	{
		MarkerData->MarkedActor.SoftObjectPtr.WeakPtr = FWeakObjectPtr(MarkerRequest.MarkedActor);
		MarkerData->MarkedActorClass.SoftObjectPtr.WeakPtr = FWeakObjectPtr(MarkerRequest.MarkedActor->GetClass());
	}

	MarkerData->bHasCustomDisplayInfo = false;

	for (auto TeamMember : PlayerState->PlayerTeam->TeamMembers)
	{
		if (TeamMember == PlayerController)
			continue;

		if (auto PlayerController = Cast<AFortPlayerControllerAthena>(TeamMember))
		{
			if (PlayerController->MarkerComponent)
				PlayerController->MarkerComponent->ClientAddMarker(MarkerData);
		}
	}
}

void UAthenaMarkerComponent::ClientCancelMarker(FMarkerID MarkerID)
{
	this->ProcessEvent(L"ClientCancelMarker", &MarkerID);
}

void UAthenaMarkerComponent::ClientAddMarker(FFortWorldMarkerData* MarkerData)
{
	this->ProcessEvent(L"ClientAddMarker", MarkerData);
}