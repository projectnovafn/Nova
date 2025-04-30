#include "Vehicles/FortOctopusVehicle.h"

#include "Kismet/FortKismetLibrary.h"

void AFortOctopusVehicle::ServerUpdateTowhook(FVector InNetTowhookAimDir)
{
	NetTowhookAimDir = InNetTowhookAimDir;
	OnRep_NetTowhookAimDir();

	UE_LOG(LogFort, Log, TEXT("ServerUpdateTowhook: %s"), *TowHookProjectile->GetName());

	if (!TowHookProjectile)
	{
		UE_LOG(LogFort, Log, TEXT("ServerUpdateTowhook: NULL Projectile"), *TowHookProjectile->GetName());

		if (ReplicatedAttachState.Component != NULL)
		{
			UE_LOG(LogFort, Log, TEXT("ServerUpdateTowhook: BreakTowhook"), *TowHookProjectile->GetName());
			BreakTowhook();
		}

		ReplicatedAttachState.Component = NULL;
		ReplicatedAttachState.LocalLocation = FVector();
		ReplicatedAttachState.LocalNormal = FVector();

		ReplicatedAttachState.CopyTo(&LocalAttachState);

		OnRep_ReplicatedAttachState();

		return;
	}

	FAttachedInfo& AttachedInfo = TowHookProjectile->AttachedInfo;

	UE_LOG(LogFort, Log, TEXT("ServerUpdateTowhook: %s %s %s"), *AttachedInfo.AttachedToActor->GetName(), *ReplicatedAttachState.Component->GetName(), *AttachedInfo.Hit.Component->GetName());

	if (AttachedInfo.AttachedToActor == nullptr && ReplicatedAttachState.Component != NULL)
	{
		UE_LOG(LogFort, Log, TEXT("ServerUpdateTowhook: BreakTowhook"));

		BreakTowhook();
		return;
	}

	if (auto Component = AttachedInfo.Hit.Component.Get())
	{
		UE_LOG(LogFort, Log, TEXT("ServerUpdateTowhook: Component"));

		ReplicatedAttachState.Component = Component;
		ReplicatedAttachState.LocalLocation = UKismetMathLibrary::InverseTransformLocation(Component->GetComponentToWorld(), AttachedInfo.Hit.Location);
		ReplicatedAttachState.LocalNormal = UKismetMathLibrary::InverseTransformDirection(Component->GetComponentToWorld(), AttachedInfo.Hit.Normal);

		ReplicatedAttachState.CopyTo(&LocalAttachState);

		OnRep_ReplicatedAttachState();
	}
	else
	{
		UE_LOG(LogFort, Log, TEXT("ServerUpdateTowhook: No Component"));
	}
}

void AFortOctopusVehicle::ServerSetTowhookAttachState(FNetTowhookAttachState InTowhookAttachState)
{
	InTowhookAttachState.CopyTo(&LocalAttachState);
	InTowhookAttachState.CopyTo(&ReplicatedAttachState);

	OnRep_ReplicatedAttachState();
}

void AFortOctopusVehicle::SetTowhookAttachState(UPrimitiveComponent* Component, FVector WorldLocation, FVector WorldNormal)
{
	ReplicatedAttachState.Component = Component;
	ReplicatedAttachState.LocalLocation = WorldLocation;
	ReplicatedAttachState.LocalNormal = WorldNormal;

	OnRep_ReplicatedAttachState();
}

void AFortOctopusVehicle::UpdateAimTrace(FVector TowhookAimDir, AFortPlayerController* PlayerController)
{
	static auto UpdateAimTrace = UKismetMemoryLibrary::Get<void (*)(AFortOctopusVehicle*, FVector&, AFortPlayerController*)>(L"AFortOctopusVehicle::UpdateAimTrace");

	UpdateAimTrace(this, TowhookAimDir, PlayerController);
}

void AFortOctopusVehicle::OnRep_ReplicatedAttachState()
{
	this->ProcessEvent(L"OnRep_ReplicatedAttachState");
}

void AFortOctopusVehicle::OnRep_NetTowhookAimDir()
{
	this->ProcessEvent(L"OnRep_NetTowhookAimDir");
}

void AFortOctopusVehicle::DestroyTowhookProjectile()
{
	this->ProcessEvent(L"DestroyTowhookProjectile");
}

void AFortOctopusVehicle::BreakTowhook()
{
	this->ProcessEvent(L"BreakTowhook");
}