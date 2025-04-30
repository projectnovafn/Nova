#pragma once
#include "Core.h"

class ULevelSaveRecord : public UObject
{
	GENERATED_UCLASS_BODY(ULevelSaveRecord, FortniteGame)
public:

	UPROPERTY(FVector, Center)
	FVector Center;

	UPROPERTY(FRotator, Rotation)
	FRotator Rotation;
};