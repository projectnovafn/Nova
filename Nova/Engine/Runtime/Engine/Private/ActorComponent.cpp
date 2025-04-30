#include "Components/ActorComponent.h"
#include "GameFramework/Actor.h"

AActor* UActorComponent::GetOwner()
{
	AActor* ReturnValue;
	this->ProcessEvent(L"GetOwner", &ReturnValue);

	return ReturnValue;
}