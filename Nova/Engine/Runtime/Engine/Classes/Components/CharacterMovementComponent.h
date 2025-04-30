#pragma once
#include "PawnMovementComponent.h"

enum class EMovementMode : uint8
{
	None = 0,
	Walking = 1,
	NavWalking = 2,
	Falling = 3,
	Swimming = 4,
	Flying = 5,
	Custom = 6
};

class UCharacterMovementComponent : public UPawnMovementComponent
{
	GENERATED_UCLASS_BODY(UCharacterMovementComponent, Engine)
public:

	UPROPERTY(EMovementMode, MovementMode)
	EMovementMode MovementMode;
};