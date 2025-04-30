#pragma once
#include "Player/FortPlayerStatePvP.h"
#include "FortTeamInfo.h"

enum EMatchAbandonState : uint8_t
{
	None,
	Joining,
	CanAbandon,
	TeamLocked,
	EMatchAbandonState_MAX
};

class FDeathInfo
{
	GENERATED_USTRUCT_BODY(FDeathInfo, FortniteGame)
public:

	UPROPERTY_STRUCT(AActor*, FinisherOrDowner)
	AActor* FinisherOrDowner;

	UPROPERTY_STRUCT(bool, bDBNO)
	bool bDBNO;

	UPROPERTY_STRUCT(uint8, DeathCause)
	uint8 DeathCause;

	UPROPERTY_STRUCT(float, Distance)
	float Distance;

	UPROPERTY_STRUCT(FVector, DeathLocation)
	FVector DeathLocation;

	UPROPERTY_STRUCT(bool, bInitialized)
	bool bInitialized;

	UPROPERTY_STRUCT(FGameplayTagContainer, DeathTags)
	FGameplayTagContainer DeathTags;
};

class FFortRespawnData
{
	GENERATED_USTRUCT_BODY(FFortRespawnData, FortniteGame)
public:

	UPROPERTY_STRUCT_BOOL(bRespawnDataAvailable)
	bool bRespawnDataAvailable;

	UPROPERTY_STRUCT_BOOL(bClientIsReady)
	bool bClientIsReady;

	UPROPERTY_STRUCT_BOOL(bServerIsReady)
	bool bServerIsReady;

	UPROPERTY_STRUCT(FVector, RespawnLocation)
	FVector RespawnLocation;

	UPROPERTY_STRUCT(FRotator, RespawnRotation)
	FRotator RespawnRotation;

	UPROPERTY_STRUCT(float, RespawnCameraDistance)
	float RespawnCameraDistance;
};

class AFortPlayerStateAthena : public AFortPlayerStatePvP
{
	GENERATED_UCLASS_BODY(AFortPlayerStateAthena, FortniteGame)
public:

	UPROPERTY(FFortRespawnData, RespawnData)
	FFortRespawnData RespawnData;

	UPROPERTY_BOOL(bInAircraft)
	bool bInAircraft;

	UPROPERTY(uint8, TeamIndex)
	uint8 TeamIndex;

	UPROPERTY(uint8, SquadId)
	uint8 SquadId;

	UPROPERTY(uint8, StormSurgeEffectCount)
	uint8 StormSurgeEffectCount;

	UPROPERTY(FDeathInfo, DeathInfo)
	FDeathInfo DeathInfo;

	UPROPERTY(int, TeamKillScore)
	int TeamKillScore;

	UPROPERTY(int, KillScore)
	int KillScore;

	UPROPERTY(int, Place)
	int Place;
	
	UPROPERTY(int, TeamScore)
	int TeamScore;

	UPROPERTY(int, TeamScorePlacement)
	int TeamScorePlacement;

	UPROPERTY(int, TotalPlayerScore)
	int TotalPlayerScore;

	UPROPERTY_NOTIFY(int, RebootCounter)
	int RebootCounter;

	UPROPERTY(EMatchAbandonState, MatchAbandonState)
	EMatchAbandonState MatchAbandonState;

	UPROPERTY(AFortTeamInfo*, PlayerTeam)
	AFortTeamInfo* PlayerTeam;

	UPROPERTY(AFortPlayerStateAthena*, SpectatingTarget)
	AFortPlayerStateAthena* SpectatingTarget;

	void OnRep_MatchAbandonState(EMatchAbandonState PrevMatchAbandonState);
	void OnRep_TeamKillScore();
	void OnRep_Kills();
	void OnRep_Place();
	void OnRep_TeamScore();
	void OnRep_TeamScorePlacement();
	void OnRep_SquadId();
	void OnRep_DeathInfo();
	void OnRep_StormSurgeEffectCount();
	void OnRep_SpectatingTarget();

	void ServerSetInAircraft(bool bNewInAircraft);

	void ClientReportTeamKill(int TeamKills);
	void ClientReportKill(AFortPlayerStateAthena* Player);
	void ClientReportDBNO(AFortPlayerStateAthena* Player);
	void ClientNotifyMatchEntered(FString EventId, FString EventWindowId);

	static uint8 ToDeathCause(FGameplayTagContainer& DeathTags, bool bWasDBNO);
};