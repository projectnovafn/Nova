#include "Engine/NetConnection.h"
#include "Athena/FortGameModeAthena.h"

#include "Engine/World.h"
#include "Engine/NetDriver.h"

UWorld* UNetConnection::GetWorld()
{
	UWorld* World = nullptr;
	if (Driver)
	{
		World = Driver->World;
	}

	if (!World && OwningActor)
	{
		World = OwningActor->GetWorld();
	}

	return World;
}

void UNetConnection::SetClientLoginState(EClientLoginState State)
{
	if (State == EClientLoginState::ReceivedJoin)
	{
		if (auto GameMode = Cast<AFortGameModeAthena>(GetWorld()->AuthorityGameMode))
		{
			if (auto PlayerController = Cast<APlayerController>(OwningActor))
				GameMode->PostLogin(PlayerController);
		}
	}
}

void UNetConnection::Close()
{
	static auto Close = UKismetMemoryLibrary::Get<void (*)(UNetConnection*)>(L"UNetConnection::Close");

	Close(this);
}