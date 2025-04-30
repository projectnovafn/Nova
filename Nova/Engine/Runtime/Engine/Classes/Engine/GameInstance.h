#pragma once
#include "CoreUObject.h"
#include "LocalPlayer.h"

class UGameInstance : public UObject
{
	GENERATED_UCLASS_BODY(UGameInstance, Engine)
public:

	UPROPERTY(TArray<ULocalPlayer*>, LocalPlayers)
	TArray<ULocalPlayer*> LocalPlayers;
};