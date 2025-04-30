#pragma once

#include "Pawns/FortPlayerPawnAthena.h"
#include "UObject/Stack.h"
#include "ItemDefinitions/CustomCharacterPart.h"

class AFortPlayerPawnAthenaPatch
{
private:

	static void ServerHandlePickupHook(AFortPlayerPawnAthena*, AFortPickup*, float, FVector, bool);
	static void ServerHandlePickupWithSwapHook(AFortPlayerPawnAthena*, AFortPickup*, FGuid, float, FVector, bool);
	static void ServerHandlePickupWithRequestedSwapHook(AFortPlayerPawnAthena*, AFortPickup*, FGuid, float, FVector, bool);
	static void ServerSendZiplineStateHook(AFortPlayerPawnAthena*, FZiplinePawnState&);
	static void ServerReviveFromDBNOHook(AFortPlayerPawnAthena*, AController*);
	static void ServerChoosePartHook(AFortPlayerPawnAthena*, EFortCustomPartType, UCustomCharacterPart*);
	static void ReceiveActorBeginOverlapHook(AActor*, AActor*);
	static void NetMulticast_Athena_BatchedDamageCuesHook(AFortPlayerPawnAthena*, FFrame&, void*);
	static void OnRep_InVehicleHook(AFortPlayerPawnAthena*);
	static void MovingEmoteStoppedHook(AFortPawn*, FFrame&);
	static FRotator* GetViewRotationHook(AFortPlayerPawn*, FRotator*);

public:

	static void Init();
};