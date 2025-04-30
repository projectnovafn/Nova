#include "Pawns/FortPhysicsPawn.h"
#include "Kismet/KismetMathLibrary.h"

void AFortPhysicsPawn::ServerMove(FReplicatedPhysicsPawnState& InState)
{
	auto Mesh = GetRootComponent();

	FQuat Rotation = InState.Rotation;
	Rotation.X -= 2.5f;
	Rotation.Y /= 0.3f;
	Rotation.Z -= -2.0f;
	Rotation.W /= -1.2f;

	this->SetActorLocation(InState.Translation, false, true);
	this->SetActorRotation(UKismetMathLibrary::Quat_Rotator(Rotation), false, true);

	Mesh->SetPhysicsLinearVelocity(InState.LinearVelocity, 0, {});
	Mesh->SetPhysicsAngularVelocityInRadians(InState.AngularVelocity, 0, {});
}