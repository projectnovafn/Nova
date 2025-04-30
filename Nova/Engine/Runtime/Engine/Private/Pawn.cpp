#include "GameFramework/Pawn.h"

FRotator APawn::GetControlRotation()
{
	FRotator ReturnValue;
	this->ProcessEvent(L"GetControlRotation", &ReturnValue);

	return ReturnValue;
}