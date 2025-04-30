#include "Weapons/FortDecoTool.h"

#include "Building/BuildingSMActor.h"
#include "Building/BuildingTrap.h"

#include "Athena/Player/FortPlayerControllerAthena.h"
#include "Athena/Player/FortPlayerStateAthena.h"

#include "ItemDefinitions/FortDecoItemDefinition.h"

void AFortDecoTool::OnRep_ItemDefinition()
{
	this->ProcessEvent(L"OnRep_ItemDefinition");
}

void AFortDecoTool::CallServerSpawnDeco(FVector Location, FRotator Rotation, ABuildingSMActor* AttachedActor, EBuildingAttachmentType InBuildingAttachmentType)
{
	struct
	{
		FVector Location; 
		FRotator Rotation;
		ABuildingSMActor* AttachedActor; 
		EBuildingAttachmentType InBuildingAttachmentType;
	} params(Location, Rotation, AttachedActor, InBuildingAttachmentType);

	this->ProcessEvent(L"ServerSpawnDeco", &params);
}

void AFortDecoTool::ServerSpawnDeco(FVector Location, FRotator Rotation, ABuildingSMActor* AttachedActor, EBuildingAttachmentType InBuildingAttachmentType)
{
	if (auto Pawn = Cast<AFortPlayerPawn>(GetOwner()))
	{
		if (auto Controller = Cast<AFortPlayerController>(Pawn->Controller))
		{
			if (auto Deco = GetWorld()->SpawnActor<ABuildingTrap>(Location, Rotation, Cast<UFortDecoItemDefinition>(ItemDefinition)->GetBlueprintClass()))
			{
				Deco->TrapData = ItemDefinition;

				Deco->AttachedTo = AttachedActor;
				Deco->OnRep_AttachedTo();

				if (auto PlayerState = Cast<AFortPlayerStateAthena>(Controller->PlayerState))
					Deco->TeamIndex = PlayerState->TeamIndex;

				Deco->OnRep_Team();
				Deco->InitializeKismetSpawnedBuildingActor(Deco, Controller, true);

				if (auto ItemEntry = Controller->WorldInventory->FindItemEntry(ItemDefinition))
				{
					if (Controller->WorldInventory->RemoveItem(ItemEntry->ItemGuid, 1))
						Controller->WorldInventory->Update();
				}
			}
		}
	}
}

void AFortDecoTool::ServerCreateBuildingAndSpawnDeco(FVector BuildingLocation, FRotator BuildingRotation, FVector Location, FRotator Rotation, EBuildingAttachmentType InBuildingAttachmentType)
{
	static auto WoodFloorClass = LoadObject<UClass>(L"/Game/Building/ActorBlueprints/Player/Wood/L1/PBWA_W1_Floor.PBWA_W1_Floor_C");
	static auto BrickFloorClass = LoadObject<UClass>(L"/Game/Building/ActorBlueprints/Player/Stone/L1/PBWA_S1_Floor.PBWA_S1_Floor_C");
	static auto MetalFloorClass = LoadObject<UClass>(L"/Game/Building/ActorBlueprints/Player/Metal/L1/PBWA_M1_Floor.PBWA_M1_Floor_C");

	if (auto Pawn = Cast<AFortPlayerPawn>(GetOwner()))
	{
		if (auto Controller = Cast<AFortPlayerController>(Pawn->Controller))
		{
			auto RemoteBuildingMaterial = Controller->BroadcastRemoteClientInfo->RemoteBuildingMaterial;

			UClass* BuildingClass = nullptr;

			switch (RemoteBuildingMaterial)
			{
			case EFortResourceType::Wood:
				BuildingClass = WoodFloorClass;
				break;

			case EFortResourceType::Stone:
				BuildingClass = BrickFloorClass;
				break;

			case EFortResourceType::Metal:
				BuildingClass = MetalFloorClass;
				break;
			}

			if (!BuildingClass)
				return;

			if (auto BuildingActor = Controller->CreateBuildingActor(BuildingLocation, BuildingRotation, false, BuildingClass))
			{
				CallServerSpawnDeco(Location, Rotation, BuildingActor, InBuildingAttachmentType);
			}
		}
	}
}