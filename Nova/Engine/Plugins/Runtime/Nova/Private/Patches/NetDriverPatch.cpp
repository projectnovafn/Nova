#include "Patches/NetDriverPatch.h"

#include "KismetDetoursLibrary.h"

UDetour* TickFlushDetour = NULL;

static void (*_TickFlush)(UNetDriver*, float);

void UNetDriverPatch::TickFlushHook(UNetDriver* NetDriver, float DeltaSeconds)
{
	NetDriver->TickFlush(DeltaSeconds);

	_TickFlush(NetDriver, DeltaSeconds);
}

void UNetDriverPatch::Init()
{
	_TickFlush = UKismetMemoryLibrary::Get<decltype(_TickFlush)>(L"UNetDriver::TickFlush");

	TickFlushDetour = new UDetour();
	TickFlushDetour->Init(_TickFlush, TickFlushHook, EDetourType::Detour);
	TickFlushDetour->Commit();
}