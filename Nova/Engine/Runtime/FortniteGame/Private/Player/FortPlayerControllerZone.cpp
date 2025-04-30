#include "Player/FortPlayerControllerZone.h"

void AFortPlayerControllerZone::SpectateOnDeath()
{
	this->ProcessEvent(L"SpectateOnDeath");
}

void AFortPlayerControllerZone::ClientOnPawnRevived(AController* EventInstigator)
{
	this->ProcessEvent(L"ClientOnPawnRevived", &EventInstigator);
}

void AFortPlayerControllerZone::ClientOnPawnSpawned()
{
	this->ProcessEvent(L"ClientOnPawnSpawned");
}

void AFortPlayerControllerZone::ClientClearDeathNotification()
{
	this->ProcessEvent(L"ClientClearDeathNotification");
}