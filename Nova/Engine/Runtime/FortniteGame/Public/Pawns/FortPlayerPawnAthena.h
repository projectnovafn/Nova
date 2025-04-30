#pragma once

#include "Pawns/FortPlayerPawn.h"
#include "UObject/Stack.h"
#include "ItemDefinitions/CustomCharacterPart.h"

class FZiplinePawnState
{
	GENERATED_USTRUCT_BODY(FZiplinePawnState, FortniteGame)

public:

	UPROPERTY_STRUCT(class AFortAthenaZipline*, Zipline)
	class AFortAthenaZipline* Zipline;

	UPROPERTY_STRUCT(bool, bIsZiplining)
	bool bIsZiplining;

	UPROPERTY_STRUCT(bool, bJumped)
	bool bJumped;

	UPROPERTY_STRUCT(int, AuthoritativeValue)
	int AuthoritativeValue;

	UPROPERTY_STRUCT(struct FVector, SocketOffset)
	struct FVector SocketOffset;

	UPROPERTY_STRUCT(float, TimeZipliningBegan)
	float TimeZipliningBegan;

	UPROPERTY_STRUCT(float, TimeZipliningEndedFromJump)
	float TimeZipliningEndedFromJump;
};

class UCustomCharacterPart;

class AFortPlayerPawnAthena : public AFortPlayerPawn
{
	GENERATED_UCLASS_BODY(AFortPlayerPawnAthena, FortniteGame)
public:

	UPROPERTY(FZiplinePawnState, ZiplineState)
	FZiplinePawnState ZiplineState;

	UPROPERTY(float, LastFallDistance)
	float LastFallDistance;

	void ServerSendZiplineState(FZiplinePawnState& InZiplineState);
	void ServerReviveFromDBNO(AController* EventInstigator);
	void ServerChoosePart(EFortCustomPartType Part, UCustomCharacterPart* ChosenCharacterPart);

	void TeleportToSkyDive(float HeightAboveGround);

	void HandleZiplineStateChanged();
	void ReceiveActorBeginOverlap(AActor* OtherActor);

	void NetMulticast_Athena_BatchedDamageCues();
};