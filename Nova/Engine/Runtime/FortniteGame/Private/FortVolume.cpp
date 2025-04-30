#include "FortVolume.h"

void AFortVolume::ServerClearVolume()
{
	this->ProcessEvent(L"ServerClearVolume");
}

TArray<AActor*> AFortVolume::GetActorsWithinVolumeByClass(UClass* ActorClass)
{
	struct Params 
	{
		UClass* ActorClass;
		TArray<AActor*> ReturnValue;
	} params (ActorClass);

	this->ProcessEvent(L"GetActorsWithinVolumeByClass", &params);

	return params.ReturnValue;
}