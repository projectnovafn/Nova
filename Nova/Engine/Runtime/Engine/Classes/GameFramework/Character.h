#pragma once
#include "Pawn.h"

#include "Components/CharacterMovementComponent.h"

class ACharacter : public APawn
{
	GENERATED_UCLASS_BODY(ACharacter, Engine)
public:

	UPROPERTY(UCharacterMovementComponent*, CharacterMovement)
	UCharacterMovementComponent* CharacterMovement;
};