#include "UACServer.h"
#include "UACNetComponent.h"
#include "Engine/GameplayStatics.h"

void FUACServer::RegisterPlayer(AFortPlayerController* Player)
{
	UE_LOG(LogUAC, Log, TEXT("Received RegisterPlayer for %s"), *Player->GetFullName());

	UUACNetComponent* Comp = UGameplayStatics::SpawnObject<UUACNetComponent>(Player);
	Comp->SendClientHello();
}
