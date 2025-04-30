#include "Patches/BuildingSMActorPatch.h"
#include "KismetDetoursLibrary.h"

#include <Windows.h>

static __int64 (*_OnDamageServer)(ABuildingActor* BuildingActor, float Damage, __int64 DamageTags, FVector Momentum, __int64 HitInfo, AController* InstigatedBy, AActor* DamageCauser, __int64 EffectContext);

__int64 ABuildingSMActorPatch::OnDamageServerHook(ABuildingActor* BuildingActor, float Damage, __int64 DamageTags, FVector Momentum, __int64 HitInfo, AController* InstigatedBy, AActor* DamageCauser, __int64 EffectContext)
{
	if (auto BuildingSMActor = Cast<ABuildingSMActor>(BuildingActor))
	{
		BuildingSMActor->OnDamageServer(Damage, Momentum, InstigatedBy, DamageCauser);
	}

	return _OnDamageServer(BuildingActor, Damage, DamageTags, Momentum, HitInfo, InstigatedBy, DamageCauser, EffectContext);
}

void ABuildingSMActorPatch::Init()
{
	UKismetDetoursLibrary::AddReflectedDetour<ABuildingSMActor>(
		L"OnDamageServer",
		EReflectedDetourType::CallDetour,
		OnDamageServerHook,
		_OnDamageServer,
		0,
		{ 0x40, 0x55 }
	);
}