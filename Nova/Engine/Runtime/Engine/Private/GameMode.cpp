#include "GameFramework/GameMode.h"

void AGameMode::RestartGame()
{
	this->ProcessEvent(L"RestartGame");
}