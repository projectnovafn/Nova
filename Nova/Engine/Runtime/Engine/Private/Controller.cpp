#include "GameFramework/Controller.h"

FRotator AController::GetControlRotation()
{
	FRotator ReturnValue;
	this->ProcessEvent(L"GetControlRotation", &ReturnValue);

	return ReturnValue;
}

AActor* AController::GetViewTarget()
{
	AActor* ReturnValue;
	this->ProcessEvent(L"GetViewTarget", &ReturnValue);

	return ReturnValue;
}

void AController::ClientSetRotation(FRotator NewRotation, bool bResetCamera)
{
	struct
	{
		FRotator NewRotation;
		bool bResetCamera;
	} params(NewRotation, bResetCamera);

	this->ProcessEvent(L"ClientSetRotation", &params);
}