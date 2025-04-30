#include "Patches/WorldPatch.h"

static void (*_BeginPlay)(UWorld*);
static void (*_Tick)(UWorld*, ELevelTick, float);

UDetour* BeginPlayDetour = NULL;
UDetour* TickDetour = NULL;

bool UWorldPatch::ListenHook(UWorld* World)
{
	FURL URL{};
	URL.Port = 7777;
	
	return World->Listen(URL);
}

void UWorldPatch::BeginPlayHook(UWorld* World)
{
	UE_LOG(LogWorld, Log, TEXT("UWorld::BeginPlay: %s"), *World->GetName());

	UWorldPatch::ListenHook(World);

	_BeginPlay(World);
}

ENetMode UWorldPatch::GetNetModeHook(UWorld* World)
{
	return World->GetNetMode();
}

void UWorldPatch::TickHook(UWorld* World, ELevelTick TickType, float DeltaSeconds)
{
	GWorld = World;

	World->Tick(TickType, DeltaSeconds);

	_Tick(World, TickType, DeltaSeconds);
}

void UWorldPatch::Init()
{
	auto ListenCall = UKismetMemoryLibrary::Get<void*>(L"UWorld::Listen");
	auto GetNetMode = UKismetMemoryLibrary::Get<void*>(L"UWorld::GetNetMode");
	_BeginPlay = UKismetMemoryLibrary::Get<decltype(_BeginPlay)>(L"UWorld::BeginPlay");
	_Tick = UKismetMemoryLibrary::Get<decltype(_Tick)>(L"UWorld::Tick");

	auto ListenDetour = new UDetour();
	auto GetNetModeDetour = new UDetour();

	if (ListenCall != NULL && _BeginPlay == NULL)
	{
		ListenDetour->Init(ListenCall, UWorldPatch::ListenHook, EDetourType::Rel32Call);
		ListenDetour->Commit();
	}
	else
	{
		BeginPlayDetour = new UDetour();
		BeginPlayDetour->Init(_BeginPlay, BeginPlayHook);
		BeginPlayDetour->Commit();
	}

	TickDetour = new UDetour();
	TickDetour->Init(_Tick, TickHook);
	TickDetour->Commit();

	GetNetModeDetour->Init(GetNetMode, UWorldPatch::GetNetModeHook);
	GetNetModeDetour->Commit();
}