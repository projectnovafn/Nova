#pragma once
#include "Actor.h"

class AController;
class APlayerState;

class APawn : public AActor
{
	GENERATED_UCLASS_BODY(APawn, Engine)
public:

	UPROPERTY(AController*, Controller)
	AController* Controller;

	UPROPERTY(APlayerState*, PlayerState)
	APlayerState* PlayerState;

	FRotator GetControlRotation();
};