#pragma once
#include "GameFramework/Pawn.h"

class FReplicatedPhysicsPawnState
{
public:
	FVector Translation;
private:
	unsigned char UnknownData00[0x4];
public:
	FQuat Rotation;
	FVector LinearVelocity;
	FVector AngularVelocity;
	uint16 SyncKey;
};

class AFortPhysicsPawn : public APawn
{
	GENERATED_UCLASS_BODY(AFortPhysicsPawn, FortniteGame)
public:

	void ServerMove(FReplicatedPhysicsPawnState& InState);
};