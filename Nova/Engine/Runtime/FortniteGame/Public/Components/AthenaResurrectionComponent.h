#pragma once
#include "Components/ActorComponent.h"
#include "GameFramework\OnlineReplStructs.h"

class AFortPlayerStart;

class UAthenaResurrectionComponent : public UActorComponent
{
	GENERATED_UCLASS_BODY(UAthenaResurrectionComponent, FortniteGame)
public:

	UPROPERTY(TArray<FUniqueNetIdRepl>, PlayerIdsForResurrection)
	TArray<FUniqueNetIdRepl> PlayerIdsForResurrection;

	UPROPERTY(TWeakObjectPtr<AFortPlayerStart>, ResurrectionLocation)
	TWeakObjectPtr<AFortPlayerStart> ResurrectionLocation;

	void OnRep_PlayerIdsForResurrection();
};