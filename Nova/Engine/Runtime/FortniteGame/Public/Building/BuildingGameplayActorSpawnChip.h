#pragma once
#include "Building/BuildingGameplayActor.h"

class AFortPlayerControllerAthena;
class AFortPlayerStateAthena;

struct FRebootCardReplicatedState
{
	GENERATED_USTRUCT_BODY(FRebootCardReplicatedState, FortniteGame)
public:

	UPROPERTY_STRUCT(float, ChipExpirationServerStartTime)
	float ChipExpirationServerStartTime;

	UPROPERTY_STRUCT(AFortPlayerStateAthena*, PlayerState)
	AFortPlayerStateAthena* PlayerState;
};

class ABuildingGameplayActorSpawnChip : public ABuildingGameplayActor
{
	GENERATED_UCLASS_BODY(ABuildingGameplayActorSpawnChip, FortniteGame)
public:

	UPROPERTY(AFortPlayerControllerAthena*, OwnerPlayerController)
	AFortPlayerControllerAthena* OwnerPlayerController;

	UPROPERTY(FUniqueNetIdRepl, OwnerPlayerId)
	FUniqueNetIdRepl OwnerPlayerId;

	UPROPERTY(AFortPlayerStateAthena*, OwnerPlayerState)
	AFortPlayerStateAthena* OwnerPlayerState;

	UPROPERTY(FRebootCardReplicatedState, RebootCardReplicatedState)
	FRebootCardReplicatedState RebootCardReplicatedState;

	UPROPERTY_BOOL(bAutoAcquireSpawnChip)
	bool bAutoAcquireSpawnChip;

	UPROPERTY(uint8, SquadId)
	uint8 SquadId;
};