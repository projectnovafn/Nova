#include "Patches/ActorPatch.h"
#include "KismetDetoursLibrary.h"

ENetMode AActorPatch::GetNetModeHook(AActor* Actor)
{
    return Actor->GetNetMode();
}

void AActorPatch::Init()
{
    auto GetNetMode = UKismetMemoryLibrary::Get<void*>(L"AActor::GetNetMode");
    auto GetNetModeDetour = new UDetour();

    GetNetModeDetour->Init(GetNetMode, AActorPatch::GetNetModeHook, EDetourType::Detour);
    GetNetModeDetour->Commit();
}