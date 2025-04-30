#include "Athena/Creative/FortCreativePortalManager.h"
#include "Athena/Creative/FortAthenaCreativePortal.h"

#include "Athena/Player/FortPlayerStateAthena.h"
#include "Athena/Player/FortPlayerControllerAthena.h"
#include "Pawns/FortPlayerPawnAthena.h"

#include "Athena/FortGameStateAthena.h"

#include "Components/FortLevelSaveComponent.h"
#include "Components/PlaysetLevelStreamComponent.h"
#include "Components/LevelSaveComponent.h"
#include "Components/FortMinigameVolumeComponent.h"

#include "Kismet/KismetTextLibrary.h"
#include "Kismet/FortKismetLibrary.h"

#include "LevelRecordSpawner.h"
#include "FortMinigameSettingsBuilding.h"

#include "ItemDefinitions/FortCreativeRealEstatePlotItem.h"

void AFortCreativePortalManager::LoadPlot(AFortPlayerControllerAthena* PlayerController, UFortCreativeRealEstatePlotItem* Plot)
{
	if (!PlayerController || !Plot)
		return;

	auto Portal = PlayerController->OwnedPortal;
	auto Volume = Portal->LinkedVolume;

	UE_LOG(LogFort, Log, TEXT("Portal: %s, Volume: %s"), *Portal->GetName(), *Volume->GetName());

	if (!Portal || !Volume)
		return;

	auto PlotItemDefinition = Cast<UFortCreativeRealEstatePlotItemDefinition>(Plot->ItemDefinition);

	UE_LOG(LogFort, Log, TEXT("PlotItemDefinition: %s"), *PlotItemDefinition->GetName());

	if (!PlotItemDefinition)
		return;

	auto IslandPlayset = PlotItemDefinition->BasePlayset.Get();

	UE_LOG(LogFort, Log, TEXT("PlotItemDefinition: %s"), *PlotItemDefinition->GetName());
	UE_LOG(LogFort, Log, TEXT("BasePlayset: %s"), *PlotItemDefinition->BasePlayset.ToString());
	UE_LOG(LogFort, Log, TEXT("IslandPlayset: %s"), *IslandPlayset->GetName());

	/*if (IslandPlayset && !IslandPlayset->LevelSaveRecord)
	{
		auto Objects = UFortKismetLibrary::JonLHack_GetAllObjectsOfClassFromPath(L"/Game/Playsets", UObject::StaticClass());

		for (auto Object : Objects)
		{
			auto Playset = Cast<UFortPlaysetItemDefinition>(Object);

			if (Playset->GetName() == IslandPlayset->GetName() && Playset->LevelSaveRecord)
			{
				IslandPlayset = Playset;
			}
		}
	}*/

	if (!IslandPlayset)
		return;

	UE_LOG(LogFort, Log, TEXT("LevelSaveRecord: %s"), *IslandPlayset->LevelSaveRecord->GetName());

	// kick all players out

	if (auto LevelSaveComponent = Volume->GetComponent<UFortLevelSaveComponent>())
	{
		LevelSaveComponent->ResetLevelRecord();
	}

	Portal->bPortalOpen = false;
	Portal->OnRep_PortalOpen();

	Volume->VolumeState = EVolumeState::Initializing;

	Portal->IslandInfo.AltTitle = Portal->LoadingText;
	Portal->OnRep_IslandInfo();

	if (auto PlaysetLevelStreamComp = Volume->GetComponent<UPlaysetLevelStreamComponent>())
	{
		Volume->ServerClearVolume();
		PlaysetLevelStreamComp->UnloadPlaysetAsync();

		if (auto LevelSaveComponent = Volume->GetComponent<UFortLevelSaveComponent>())
		{
			LevelSaveComponent->StopPeriodicSaveTimer();
		}
	}

	QueueTask(ENamedThreads::GameThread, 10.0f, [this, Volume, IslandPlayset, Portal, Plot]
	{
		if (auto PlaysetLevelStreamComp = Volume->GetComponent<UPlaysetLevelStreamComponent>())
		{
			Volume->CurrentPlayset = IslandPlayset;

			PlaysetLevelStreamComp->SetPlayset(IslandPlayset);
			PlaysetLevelStreamComp->LoadPlaysetAsync();

			//if (Plot->LastUsedDate.IsValid())
			{
				if (auto LevelSaveComponent = Volume->GetComponent<UFortLevelSaveComponent>())
				{
					LevelSaveComponent->LoadedPlot = Plot;
					LevelSaveComponent->LoadFromDssAsync(true);
				}
			}
			/*else if (IslandPlayset->LevelSaveRecord)
			{
				if (auto GameState = Cast<AFortGameStateAthena>(GetWorld()->GameState))
				{
					IslandPlayset->LevelSaveRecord->Outer = Volume;
					IslandPlayset->LevelSaveRecord->Center = Volume->GetActorLocation();
					IslandPlayset->LevelSaveRecord->Rotation = Volume->GetActorRotation();

					GameState->VolumeManager->ApplyLevelSaveToWorld(IslandPlayset->LevelSaveRecord, true);

					if (auto LevelSaveComponent = Volume->GetComponent<UFortLevelSaveComponent>())
					{
						LevelSaveComponent->LoadedPlot = Plot;
						LevelSaveComponent->LoadFromDssAsync(true);
					}
				}
			}*/
		}

		if (auto LevelSaveComponent = Volume->GetComponent<UFortLevelSaveComponent>())
		{
			LevelSaveComponent->LoadedPlot = Plot;
			LevelSaveComponent->bIsLoaded = true;
			LevelSaveComponent->StartPeriodicSaveTimer();
		}

		Volume->VolumeState = EVolumeState::Ready;

		Portal->IslandInfo.AltTitle = FText(Plot->IslandTitle);
		Portal->OnRep_IslandInfo();

		Portal->bPortalOpen = true;
		Portal->OnRep_PortalOpen();
	});
}

void AFortCreativePortalManager::AssignPortal(AFortPlayerControllerAthena* PlayerController)
{
	if (!AvailablePortals.IsValidIndex(0))
		return;

	auto Portal = AvailablePortals[0];
	auto PlayerState = Cast<AFortPlayerStateAthena>(PlayerController->PlayerState);

	if (!Portal)
		return;

	this->MarkPortalUsed(Portal);

	Portal->OwningPlayer = PlayerState->UniqueId;
	Portal->IslandInfo.AccountId = PlayerController->MyPlayerInfo->PlayerID->ToString();
	Portal->OnRep_IslandInfo();

	PlayerController->OwnedPortal = Portal;

	auto Volume = Portal->LinkedVolume;

	if (!Volume)
		return;

	if (auto LevelSaveComponent = Volume->GetComponent<UFortLevelSaveComponent>())
	{
		LevelSaveComponent->AccountIdOfOwner = PlayerState->UniqueId;
	}
}

void AFortCreativePortalManager::Setup()
{
	for (auto& Portal : AllPortals)
	{
		Portal->Setup();
	}
}

void AFortCreativePortalManager::ReleasePortal(AFortAthenaCreativePortal* PortalToRelease)
{
	this->ProcessEvent(L"ReleasePortal", &PortalToRelease);
}

void AFortCreativePortalManager::MarkPortalUsed(AFortAthenaCreativePortal* PortalUsed)
{
	this->ProcessEvent(L"MarkPortalUsed", &PortalUsed);
}