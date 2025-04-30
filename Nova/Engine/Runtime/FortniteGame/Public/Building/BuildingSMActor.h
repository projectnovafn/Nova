#pragma once

#include "BuildingActor.h"
#include "GameplayTagContainer.h"

#include "Engine/CurveTable.h"

class AFortPlayerStateZone;
class AFortPlayerState;

enum class EFortResourceType : uint8
{
	Wood = 0,
	Stone = 1,
	Metal = 2,
	Permanite = 3,
	None = 4
};

enum class EBuildingReplacementType : int64
{
	None = 0,
	Edited = 1,
	Upgrade = 2
};

class ABuildingSMActor : public ABuildingActor
{
	GENERATED_UCLASS_BODY(ABuildingSMActor, FortniteGame)

public:

	UPROPERTY(FCurveTableRowHandle, BuildingResourceAmountOverride)
	FCurveTableRowHandle BuildingResourceAmountOverride;

	UPROPERTY_NOTIFY(AFortPlayerStateZone*, EditingPlayer)
	AFortPlayerStateZone* EditingPlayer;

	UPROPERTY(EFortResourceType, ResourceType)
	EFortResourceType ResourceType;

	void SetMirrored(bool bIsMirrored);

	void OnDamageServer(float Damage, FVector Momentum, AController* InstigatedBy, AActor* DamageCauser);

	void RepairBuilding(AFortPlayerController* RepairingController, int ResourcesSpent);

	ABuildingSMActor* ReplaceBuildingActor(EBuildingReplacementType ReplacementType, UClass* BuildingClass, int BuildingLevel, int RotationIterations, bool bMirrored, AFortPlayerController* PlayerController);

	void SetEditingPlayer(AFortPlayerState* PlayerState);
};