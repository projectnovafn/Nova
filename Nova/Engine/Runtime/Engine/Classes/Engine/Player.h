#pragma once
#include "CoreUObject.h"

#include "GameFramework/PlayerController.h"

class UPlayer : public UObject
{
	GENERATED_UCLASS_BODY(UPlayer, Engine)
public:

	UPROPERTY(APlayerController*, PlayerController)
	APlayerController* PlayerController;
};