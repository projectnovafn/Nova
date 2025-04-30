#pragma once

#include "Components/ActorComponent.h"
#include "Internationalization/Text.h"

class AFortPlayerState;
class UFortItemDefinition;

enum class EFortWorldMarkerType : uint8
{
	None = 0,
	Location = 1,
	Enemy = 2,
	Item = 3,
	SpecialLocal = 4,
	MAX = 5
};

enum class ECancelMarkerReason : uint8
{
	Ping = 0,
	MapOrDeath = 1,
	ECancelMarkerReason_MAX = 2
};

struct FMarkerID
{
	int PlayerID;
	int InstanceID;
};

class FMarkedActorDisplayInfo
{
	GENERATED_USTRUCT_BODY(FMarkedActorDisplayInfo, FortniteGame)

public:

	UPROPERTY_STRUCT(FText, DisplayName)
	FText DisplayName;

	UPROPERTY_STRUCT(TSoftObjectPtr<UObject>, Icon)
	TSoftObjectPtr<UObject> Icon;
};

class FFortWorldMarkerData
{
	GENERATED_USTRUCT_BODY(FFortWorldMarkerData, FortniteGame)

public:

	UPROPERTY_STRUCT(FMarkerID, MarkerID)
	FMarkerID MarkerID;

	UPROPERTY_STRUCT(AFortPlayerState*, Owner)
	AFortPlayerState* Owner;

	UPROPERTY_STRUCT(EFortWorldMarkerType, MarkerType)
	EFortWorldMarkerType MarkerType;

	UPROPERTY_STRUCT(FVector, WorldPosition)
	FVector WorldPosition;

	UPROPERTY_STRUCT(FVector, WorldPositionOffset)
	FVector WorldPositionOffset;

	UPROPERTY_STRUCT(FVector, WorldNormal)
	FVector WorldNormal;

	UPROPERTY_STRUCT(UFortItemDefinition*, ItemDefinition)
	UFortItemDefinition* ItemDefinition;

	UPROPERTY_STRUCT(int, ItemCount)
	int ItemCount;

	UPROPERTY_STRUCT(TSoftObjectPtr<UClass>, MarkedActorClass)
	TSoftObjectPtr<UClass> MarkedActorClass;

	UPROPERTY_STRUCT(TSoftObjectPtr<AActor>, MarkedActor)
	TSoftObjectPtr<AActor> MarkedActor;

	UPROPERTY_STRUCT(bool, bHasCustomDisplayInfo)
	bool bHasCustomDisplayInfo;

	UPROPERTY_STRUCT(FMarkedActorDisplayInfo, CustomDisplayInfo)
	FMarkedActorDisplayInfo CustomDisplayInfo;
};

class FFortClientMarkerRequest
{
	GENERATED_USTRUCT_BODY(FFortClientMarkerRequest, FortniteGame)

public:

	UPROPERTY_STRUCT(int, InstanceID)
	int InstanceID;

	UPROPERTY_STRUCT(EFortWorldMarkerType, MarkerType)
	EFortWorldMarkerType MarkerType;

	UPROPERTY_STRUCT(FVector, WorldPosition)
	FVector WorldPosition;

	UPROPERTY_STRUCT(FVector, WorldPositionOffset)
	FVector WorldPositionOffset;

	UPROPERTY_STRUCT(FVector, WorldNormal)
	FVector WorldNormal;

	UPROPERTY_STRUCT(AActor*, MarkedActor)
	AActor* MarkedActor;
};

class UAthenaMarkerComponent : public UActorComponent
{
	GENERATED_UCLASS_BODY(UAthenaMarkerComponent, FortniteGame)

public:

	UPROPERTY(UClass*, MarkerWidgetClass)
	UClass* MarkerWidgetClass;

	UPROPERTY(UClass*, MarkerActorClass)
	UClass* MarkerActorClass;

	void ServerRemoveMapMarker(FMarkerID MarkerID, ECancelMarkerReason CancelReason);
	void ServerAddMapMarker(FFortClientMarkerRequest& MarkerRequest);

	void ClientCancelMarker(FMarkerID MarkerID);
	void ClientAddMarker(FFortWorldMarkerData* MarkerData);
};