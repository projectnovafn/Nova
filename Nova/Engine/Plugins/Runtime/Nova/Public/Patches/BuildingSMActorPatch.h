#pragma once
#include "Building/BuildingSMActor.h"

class ABuildingSMActorPatch
{
private:
	static __int64 OnDamageServerHook(ABuildingActor* BuildingActor, float Damage, __int64 DamageTags, FVector Momentum, __int64 HitInfo, AController* InstigatedBy, class AActor* DamageCauser, __int64 EffectContext);

public:

	static void Init();
};