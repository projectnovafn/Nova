#pragma once
#include "Info.h"
#include "PlayerState.h"

class AGameStateBase : public AInfo
{
	GENERATED_UCLASS_BODY(AGameStateBase, Engine)
public:

	UPROPERTY(TArray<APlayerState*>, PlayerArray)
	TArray<APlayerState*> PlayerArray;
};