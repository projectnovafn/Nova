#pragma once
#include "CoreUObject.h"

class UUACNetComponent : public UObject
{
	GENERATED_UCLASS_BODY(UUACNetComponent, UACCommon)
public:
	void SendClientHello();
};