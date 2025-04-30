#include "Patches/FortControllerComponent_AircraftPatch.h"
#include "KismetDetoursLibrary.h"

static void (*_EnterAircraftComp)(UFortControllerComponent_Aircraft*, AFortAthenaAircraft*);
static void (*_ExitAircraftComp)(UFortControllerComponent_Aircraft*);
static void (*_ServerAttemptAircraftJump)(UFortControllerComponent_Aircraft*, FRotator);

UDetour* EnterAircraftCompDetour = NULL;
UDetour* ExitAircraftCompDetour = NULL;

void UFortControllerComponent_AircraftPatch::ServerAttemptAircraftJumpHook(UFortControllerComponent_Aircraft* AircraftComp, FRotator ClientRotation)
{
	AircraftComp->ServerAttemptAircraftJump(ClientRotation);

	_ServerAttemptAircraftJump(AircraftComp, ClientRotation);
}

void UFortControllerComponent_AircraftPatch::EnterAircraftHook(UFortControllerComponent_Aircraft* AircraftComp, AFortAthenaAircraft* Aircraft)
{
	AircraftComp->EnterAircraft(Aircraft);

	_EnterAircraftComp(AircraftComp, Aircraft);
}

void UFortControllerComponent_AircraftPatch::ExitAircraftHook(UFortControllerComponent_Aircraft* AircraftComp)
{
	AircraftComp->ExitAircraft();

	_ExitAircraftComp(AircraftComp);
}

void UFortControllerComponent_AircraftPatch::Init()
{
	_EnterAircraftComp = UKismetMemoryLibrary::Get<decltype(_EnterAircraftComp)>(L"AFortPlayerControllerAthena::EnterAircraft");
	_ExitAircraftComp = UKismetMemoryLibrary::Get<decltype(_ExitAircraftComp)>(L"AFortPlayerControllerAthena::ExitAircraft");

	if (UFortControllerComponent_Aircraft::StaticClass() == NULL)
		return;

	EnterAircraftCompDetour = new UDetour();
	EnterAircraftCompDetour->Init(_EnterAircraftComp, EnterAircraftHook);
	EnterAircraftCompDetour->Commit();

	ExitAircraftCompDetour = new UDetour();
	ExitAircraftCompDetour->Init(_ExitAircraftComp, ExitAircraftHook);
	ExitAircraftCompDetour->Commit();

	UKismetDetoursLibrary::AddReflectedDetour<UFortControllerComponent_Aircraft>(
		L"ServerAttemptAircraftJump",
		EReflectedDetourType::VFSwap,
		ServerAttemptAircraftJumpHook,
		_ServerAttemptAircraftJump,
		1
	);
}