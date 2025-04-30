#pragma once
#include "Player.h"

class UNetDriver;
class UWorld;
class AActor;

enum class EClientLoginState : uint32
{
    Invalid = 0,
    LoggingIn = 1,
    Welcomed = 2,
    ReceivedJoin = 3,
    CleanedUp = 4,
};

class UNetConnection : public UPlayer
{
	GENERATED_UCLASS_BODY(UNetConnection, Engine)
public:

	UPROPERTY_BOOL(InternalAck)
	bool InternalAck;

    UPROPERTY(UNetDriver*, Driver)
    UNetDriver* Driver;

    UPROPERTY(AActor*, OwningActor)
    AActor* OwningActor;

    UWorld* GetWorld();

	void SetClientLoginState(EClientLoginState State);

    void Close();
};