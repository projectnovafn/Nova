#pragma once
#include "Info.h"
#include "Pawn.h"
#include "GameSession.h"

class AGameModeBase : public AInfo
{
	GENERATED_UCLASS_BODY(AGameModeBase, Engine)
public:

	UPROPERTY(AGameSession*, GameSession)
	AGameSession* GameSession;

	UPROPERTY(UClass*, DefaultPawnClass)
	UClass* DefaultPawnClass;
};