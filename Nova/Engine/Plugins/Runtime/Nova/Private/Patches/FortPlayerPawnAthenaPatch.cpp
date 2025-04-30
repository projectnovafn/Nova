#include "Patches/FortPlayerPawnAthenaPatch.h"
#include "KismetDetoursLibrary.h"

void (*_ServerHandlePickup)(AFortPlayerPawnAthena*, AFortPickup*, float, FVector, bool);
void (*_ServerHandlePickupWithSwap)(AFortPlayerPawnAthena*, AFortPickup*, FGuid, float, FVector, bool);
void (*_ServerHandlePickupWithRequestedSwap)(AFortPlayerPawnAthena*, AFortPickup*, FGuid, float, FVector, bool);
void (*_ServerSendZiplineState)(AFortPlayerPawnAthena*, FZiplinePawnState&);
void (*_ServerChoosePart)(AFortPlayerPawnAthena*, EFortCustomPartType, UObject*);
void (*_ReceiveActorBeginOverlap)(AActor*, AActor*);
void (*_ServerReviveFromDBNO)(AFortPlayerPawnAthena*, AController*);
void (*_NetMulticast_Athena_BatchedDamageCues)(AFortPlayerPawnAthena*, FFrame&, void*);
void (*_OnRep_InVehicle)(AFortPlayerPawnAthena*);
void (*_MovingEmoteStopped)(AFortPawn*, FFrame&);
FRotator* (*_GetViewRotation)(AFortPlayerPawn*, FRotator*);

UDetour* ReceiveActorBeginOverlapDetour = NULL;
UDetour* OnRep_InVehicleDetour = NULL;
UDetour* GetViewRotationDetour = NULL;

void AFortPlayerPawnAthenaPatch::ServerHandlePickupHook(AFortPlayerPawnAthena* Pawn, AFortPickup* Pickup, float InFlyTime, FVector InStartDirection, bool bPlayPickupSound)
{
	Pawn->ServerHandlePickup(Pickup, InFlyTime, InStartDirection, bPlayPickupSound);
}

void AFortPlayerPawnAthenaPatch::ServerHandlePickupWithSwapHook(AFortPlayerPawnAthena* Pawn, AFortPickup* Pickup, FGuid Swap, float InFlyTime, FVector InStartDirection, bool bPlayPickupSound)
{
	Pawn->ServerHandlePickupWithSwap(Pickup, Swap, InFlyTime, InStartDirection, bPlayPickupSound);
}

void AFortPlayerPawnAthenaPatch::ServerHandlePickupWithRequestedSwapHook(AFortPlayerPawnAthena* Pawn, AFortPickup* Pickup, FGuid Swap, float InFlyTime, FVector InStartDirection, bool bPlayPickupSound)
{
	Pawn->ServerHandlePickupWithRequestedSwap(Pickup, Swap, InFlyTime, InStartDirection, bPlayPickupSound);
}

void AFortPlayerPawnAthenaPatch::ServerSendZiplineStateHook(AFortPlayerPawnAthena* Pawn, FZiplinePawnState& InZiplineState)
{
	Pawn->ServerSendZiplineState(InZiplineState);
}

void AFortPlayerPawnAthenaPatch::ServerReviveFromDBNOHook(AFortPlayerPawnAthena* Pawn, AController* EventInstigator)
{
	_ServerReviveFromDBNO(Pawn, EventInstigator);

	Pawn->ServerReviveFromDBNO(EventInstigator);
}

void AFortPlayerPawnAthenaPatch::ServerChoosePartHook(AFortPlayerPawnAthena* PlayerPawn, EFortCustomPartType Part, UCustomCharacterPart* ChosenCharacterPart)
{
	PlayerPawn->ServerChoosePart(Part, ChosenCharacterPart);
}

void AFortPlayerPawnAthenaPatch::ReceiveActorBeginOverlapHook(AActor* Actor, AActor* OtherActor)
{
	if (auto Pawn = Cast<AFortPlayerPawnAthena>(OtherActor))
		Pawn->ReceiveActorBeginOverlap(Actor);

	_ReceiveActorBeginOverlap(Actor, OtherActor);
}

void AFortPlayerPawnAthenaPatch::NetMulticast_Athena_BatchedDamageCuesHook(AFortPlayerPawnAthena* Pawn, FFrame& Stack, void* ReturnValue)
{
	Pawn->NetMulticast_Athena_BatchedDamageCues();

	_NetMulticast_Athena_BatchedDamageCues(Pawn, Stack, ReturnValue);
}

void AFortPlayerPawnAthenaPatch::OnRep_InVehicleHook(AFortPlayerPawnAthena* Pawn)
{
	_OnRep_InVehicle(Pawn);

	Pawn->OnRep_InVehicle();
}

void AFortPlayerPawnAthenaPatch::MovingEmoteStoppedHook(AFortPawn* Pawn, FFrame& Stack)
{
	Pawn->MovingEmoteStopped();

	_MovingEmoteStopped(Pawn, Stack);
}

FRotator* AFortPlayerPawnAthenaPatch::GetViewRotationHook(AFortPlayerPawn* Pawn, FRotator* _)
{
	auto Rotation = _GetViewRotation(Pawn, _);
	Rotation->Roll = 0;

	return Rotation;
}

void AFortPlayerPawnAthenaPatch::Init()
{
	_ReceiveActorBeginOverlap = UKismetMemoryLibrary::Get<decltype(_ReceiveActorBeginOverlap)>(L"AActor::ReceiveActorBeginOverlap");
	_OnRep_InVehicle = UKismetMemoryLibrary::Get<decltype(_OnRep_InVehicle)>(L"AFortPlayerPawn::OnRep_InVehicle");
	_GetViewRotation = UKismetMemoryLibrary::Get<decltype(_GetViewRotation)>(L"AFortPlayerPawn::GetViewRotation");

	UKismetDetoursLibrary::AddReflectedDetour<AFortPlayerPawnAthena>(
		L"ServerHandlePickup",
		EReflectedDetourType::VFSwap,
		ServerHandlePickupHook,
		_ServerHandlePickup,
		1
	);

	UKismetDetoursLibrary::AddReflectedDetour<AFortPlayerPawnAthena>(
		L"ServerHandlePickupWithSwap",
		EReflectedDetourType::VFSwap,
		ServerHandlePickupWithSwapHook,
		_ServerHandlePickupWithSwap,
		1
	);

	UKismetDetoursLibrary::AddReflectedDetour<AFortPlayerPawnAthena>(
		L"ServerHandlePickupWithRequestedSwap",
		EReflectedDetourType::VFSwap,
		ServerHandlePickupWithRequestedSwapHook,
		_ServerHandlePickupWithRequestedSwap,
		1
	);

	UKismetDetoursLibrary::AddReflectedDetour<AFortPlayerPawnAthena>(
		L"ServerSendZiplineState",
		EReflectedDetourType::VFSwap,
		ServerSendZiplineStateHook,
		_ServerSendZiplineState,
		1
	);

	UKismetDetoursLibrary::AddReflectedDetour<AFortPlayerPawnAthena>(
		L"ServerReviveFromDBNO",
		EReflectedDetourType::VFSwap,
		ServerReviveFromDBNOHook,
		_ServerReviveFromDBNO,
		1
	);

	UKismetDetoursLibrary::AddReflectedDetour<AFortPlayerPawnAthena>(
		L"NetMulticast_Athena_BatchedDamageCues",
		EReflectedDetourType::ExecSwap,
		NetMulticast_Athena_BatchedDamageCuesHook,
		_NetMulticast_Athena_BatchedDamageCues,
		1
	);

	UKismetDetoursLibrary::AddReflectedDetour<AFortPlayerPawnAthena>(
		L"MovingEmoteStopped",
		EReflectedDetourType::ExecSwap,
		MovingEmoteStoppedHook,
		_MovingEmoteStopped
	);

	UKismetDetoursLibrary::AddReflectedDetour<AFortPlayerPawnAthena>(
		L"ServerChoosePart",
		EReflectedDetourType::VFSwap,
		ServerChoosePartHook,
		_ServerChoosePart,
		1
	);

	ReceiveActorBeginOverlapDetour = new UDetour();
	ReceiveActorBeginOverlapDetour->Init(_ReceiveActorBeginOverlap, ReceiveActorBeginOverlapHook);
	ReceiveActorBeginOverlapDetour->Commit();

	OnRep_InVehicleDetour = new UDetour();
	OnRep_InVehicleDetour->Init(_OnRep_InVehicle, OnRep_InVehicleHook);
	OnRep_InVehicleDetour->Commit();

	GetViewRotationDetour = new UDetour();
	GetViewRotationDetour->Init(_GetViewRotation, GetViewRotationHook);
	GetViewRotationDetour->Commit();
}