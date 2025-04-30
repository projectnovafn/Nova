#include "UACNetComponent.h"

void UUACNetComponent::SendClientHello()
{
	this->ProcessEvent(L"SendClientHello", nullptr);
}
