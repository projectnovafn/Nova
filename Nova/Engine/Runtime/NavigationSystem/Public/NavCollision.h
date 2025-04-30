#pragma once
#include "AI/NavCollisionBase.h"

class UNavCollision : public UNavCollisionBase
{
	GENERATED_UCLASS_BODY(UNavCollision, NavigationSystem)
public:

	UPROPERTY_BOOL(bCreateOnClient)
	bool bCreateOnClient;
};