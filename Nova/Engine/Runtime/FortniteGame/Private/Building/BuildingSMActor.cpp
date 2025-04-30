#include "Building/BuildingSMActor.h"

#include "Kismet/DataTableFunctionLibrary.h"

#include "Player/FortPlayerController.h"

#include "ItemDefinitions/FortWeaponMeleeItemDefinition.h"
#include "Kismet/FortKismetLibrary.h"

void ABuildingSMActor::SetMirrored(bool bInMirrored)
{
	ProcessEvent(L"SetMirrored", &bInMirrored);
}

void ABuildingSMActor::OnDamageServer(float Damage, FVector Momentum, AController* InstigatedBy, AActor* DamageCauser)
{
	auto PlayerController = Cast<AFortPlayerController>(InstigatedBy);
	auto Pawn = PlayerController ? PlayerController->MyFortPawn : NULL;
	auto Weapon = Cast<AFortWeapon>(DamageCauser);
	bool IsMeleeItem = Weapon ? Weapon->WeaponData->IsA<UFortWeaponMeleeItemDefinition>() : NULL;

	if (!PlayerController || !Weapon || !IsMeleeItem || !Pawn)
	{
		return;
	}

	auto AthenaResourceRatesTable = LoadObject<UCurveTable>(L"/Game/Athena/Balance/DataTables/AthenaResourceRates.AthenaResourceRates");

	auto Value = UDataTableFunctionLibrary::EvaluateCurveTableRow(AthenaResourceRatesTable, BuildingResourceAmountOverride.RowName, 0);
	auto ResourceCount = (int)round(Value / (GetMaxHealth() / Damage));

	if (ResourceCount == 0)
		return;

	auto ItemDef = UFortKismetLibrary::GetResourceItemDefinition(ResourceType);

	if (!ItemDef)
	{
		return;
	}

	bool NeedsUpdate;
	PlayerController->WorldInventory->AddItem(ItemDef, ResourceCount, NeedsUpdate);

	if (NeedsUpdate)
		PlayerController->WorldInventory->Update();

	PlayerController->ClientReportDamagedResourceBuilding(this, ResourceType, ResourceCount, GetHealth() - Damage <= 0, Damage == 100.0f);
}

void ABuildingSMActor::RepairBuilding(AFortPlayerController* RepairingController, int ResourcesSpent)
{
	struct
	{
		AFortPlayerController* RepairingController;
		int ResourcesSpent;
	} params(RepairingController, ResourcesSpent);

	this->ProcessEvent(L"RepairBuilding", &params);
}

ABuildingSMActor* ABuildingSMActor::ReplaceBuildingActor(EBuildingReplacementType ReplacementType, UClass* BuildingClass, int BuildingLevel, int RotationIterations, bool bMirrored, AFortPlayerController* PlayerController)
{
	static auto ReplaceBuildingActor = UKismetMemoryLibrary::Get<ABuildingSMActor* (*)(ABuildingSMActor*, EBuildingReplacementType, UClass*, int, int, bool, AFortPlayerController*)>(L"ABuildingSMActor::ReplaceBuildingActor");

	return ReplaceBuildingActor(this, ReplacementType, BuildingClass, BuildingLevel, RotationIterations, bMirrored, PlayerController);
}

void ABuildingSMActor::SetEditingPlayer(AFortPlayerState* PlayerState)
{
	static auto SetEditingPlayer = UKismetMemoryLibrary::Get<void (*)(ABuildingSMActor*, AFortPlayerState*)>(L"ABuildingSMActor::SetEditingPlayer");

	return SetEditingPlayer(this, PlayerState);
}