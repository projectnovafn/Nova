#pragma once
#include "GameFramework/GameSession.h"

class FPlayerReservation
{
	GENERATED_USTRUCT_BODY(FPlayerReservation, FortniteGame)
public:

	UPROPERTY_STRUCT(FString, ValidationStr)
	FString ValidationStr;

	UPROPERTY_STRUCT(FString, Platform)
	FString Platform;

	UPROPERTY_STRUCT(bool, bAllowCrossplay)
	bool bAllowCrossplay;

	UPROPERTY_STRUCT(float, ElapsedTime)
	float ElapsedTime;
};

class AFortGameSession : public AGameSession
{
	GENERATED_UCLASS_BODY(AFortGameSession, FortniteGame)
public:

	UPROPERTY(TArray<FPlayerReservation>, PlayersToRegister)
	TArray<FPlayerReservation> PlayersToRegister;
};