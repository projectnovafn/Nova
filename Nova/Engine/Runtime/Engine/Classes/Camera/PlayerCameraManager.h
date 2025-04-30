#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "Engine/EngineTypes.h"
#include "GameFramework/Actor.h"
#include "Camera/CameraTypes.h"

class APlayerCameraManager : public AActor
{
	GENERATED_UCLASS_BODY(APlayerCameraManager, Engine)

public:

	UPROPERTY(float, CameraCachePrivate)
	float Timestamp;

public:

	float GetCameraCacheTime() { return Timestamp; }
};