#include "Projectiles/FortAttachableProjectileBase.h"

void AFortAttachableProjectileBase::OnRep_ReplicatedAttachedInfo()
{
	this->ProcessEvent(L"OnRep_ReplicatedAttachedInfo");
}