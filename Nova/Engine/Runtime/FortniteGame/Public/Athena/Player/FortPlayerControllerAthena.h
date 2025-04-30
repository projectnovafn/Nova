#pragma once
#include "Player/FortPlayerControllerPvP.h"

#include "Athena/Player/FortPlayerStateAthena.h"
#include "Athena/FortAthenaAircraft.h"

#include "FortVolume.h"
#include "Internationalization/Text.h"

#include "WeakReference.h"

class UAthenaMarkerComponent;
class UAthenaResurrectionComponent;
class AFortVolume;
class AFortAthenaCreativePortal;

struct FAthenaMatchTeamStats
{
	int Place;
	int TotalPlayers;
};

struct FAthenaRewardResult
{
	GENERATED_USTRUCT_BODY(FAthenaRewardResult, FortniteGame)

	unsigned char Pad[0x40];
public:
};

struct FAthenaWeaponStats
{
	GENERATED_USTRUCT_BODY(FAthenaWeaponStats, FortniteGame)

public:

	UPROPERTY_STRUCT(FString, WeaponId)
	FString WeaponId;

	UPROPERTY_STRUCT(int*, Stats)
	int* Stats;
};

class FAthenaMatchStats
{
	GENERATED_USTRUCT_BODY(FAthenaMatchStats, FortniteGame)

public:
	UPROPERTY_STRUCT(FString, StatBucket)
	FString StatBucket;

	UPROPERTY_STRUCT(FString, MatchID)
	FString MatchID;

	UPROPERTY_STRUCT(FString, MatchEndTime)
	FString MatchEndTime;

	UPROPERTY_STRUCT(FString, MatchPlatform)
	FString MatchPlatform;

	UPROPERTY_STRUCT(int*, Stats)
	int* Stats;

	UPROPERTY_STRUCT(TArray<FAthenaWeaponStats>, WeaponStats)
	TArray<FAthenaWeaponStats> WeaponStats;
};

class FCreativeIslandData
{
	GENERATED_USTRUCT_BODY(FCreativeIslandData, FortniteGame)

public:
	UPROPERTY_STRUCT(FString, McpId)
	FString McpId;

	UPROPERTY_STRUCT(FText, IslandName)
	FText IslandName;

	UPROPERTY_STRUCT(FString, PublishedIslandCode)
	FString PublishedIslandCode;

	UPROPERTY_STRUCT(FDateTime, LastLoadedDate)
	FDateTime LastLoadedDate;

	UPROPERTY_STRUCT(FDateTime, DeletedAt)
	FDateTime DeletedAt;

	UPROPERTY_STRUCT(int, PublishedIslandVersion)
	int PublishedIslandVersion;

	UPROPERTY_STRUCT(bool, bIsDeleted)
	bool bIsDeleted;
};

class AFortPlayerControllerAthena : public AFortPlayerControllerPvP
{
	GENERATED_UCLASS_BODY(AFortPlayerControllerAthena, FortniteGame)

public:

	UPROPERTY(UAthenaMarkerComponent*, MarkerComponent)
	UAthenaMarkerComponent* MarkerComponent;

	UPROPERTY(UAthenaResurrectionComponent*, ResurrectionComponent)
	UAthenaResurrectionComponent* ResurrectionComponent;

	UPROPERTY(AFortAthenaCreativePortal*, OwnedPortal)
	AFortAthenaCreativePortal* OwnedPortal;

	UPROPERTY_NOTIFY(AFortVolume*, CreativePlotLinkedVolume)
	AFortVolume* CreativePlotLinkedVolume;

	UPROPERTY_NOTIFY(TArray<FCreativeIslandData>, CreativeIslands)
	TArray<FCreativeIslandData> CreativeIslands;

	UPROPERTY(int, MaxPlotCount)
	int MaxPlotCount;

	UPROPERTY_BOOL(bHasInitializedWorldInventory)
	bool bHasInitializedWorldInventory;

	UPROPERTY_BOOL(bMarkedAlive)
	bool bMarkedAlive;

	void ApplySiphon();
	void ServerAcknowledgePossession(APawn* P);
	void ClientStartRespawnPreparation(FVector& RespawnLoc, FRotator& RespawnRot, float RespawnCameraDist, FName InRespawnCameraBehavior, FText& HUDReasonText);
	void ServerSetRespawnData(FVector& Location, FRotator& Rotation);

	void ServerChangeName(FString S);
	bool ServerAttemptAircraftJump(FRotator ClientRotation, bool Force = false);

	void EnterAircraft(AFortAthenaAircraft* Aircraft);
	void ExitAircraft();

	void ClientOnPawnDied(FFortPlayerDeathReport& DeathInfo);

	void ClientSendTeamStatsForPlayer(FAthenaMatchTeamStats TeamStats);
	void ClientSendMatchStatsForPlayer(FAthenaMatchStats TeamStats);
	void ClientSendEndBattleRoyaleMatchForPlayer(bool bSuccess, FAthenaRewardResult Result);

	void ServerSetSpectatorWaiting(bool bWaiting);

	void RespawnPlayerAfterDeath(bool bEnterSkydiving);

	void ServerGiveCreativeItem(FFortItemEntry* ItemEntry);

	void ServerTeleportToPlaygroundLobbyIsland();
	void ServerTeleportToPlaygroundIslandDock();
	
	void ServerStartMinigame();
	void ServerUpdateActorOptions(AActor* OptionsTarget, TArray<FString>& OptionsKeys, TArray<FString>& OptionsValues);

	bool ServerLoadPlotForPortal_Validate(AFortAthenaCreativePortal* Portal, FString& PlotItemId);
	void ServerLoadPlotForPortal(AFortAthenaCreativePortal* Portal, FString& PlotItemId);

	void Tick();

	bool IsTeamDead();

	TArray<TArray<TPair<FString, int>>> GetLoadout();
	void EquipLoadout();

	TArray<TWeakObjectPtr<AFortPlayerStateAthena>>* FindTeamMembers();

	void HandleCreativeProfileUpdate();
};