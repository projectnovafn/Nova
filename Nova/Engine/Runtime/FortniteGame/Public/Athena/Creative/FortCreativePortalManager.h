#pragma once
#include "GameFramework\Info.h"

class AFortAthenaCreativePortal;
class AFortPlayerControllerAthena;
class UFortCreativeRealEstatePlotItem;

class AFortCreativePortalManager : public AInfo
{
	GENERATED_UCLASS_BODY(AFortCreativePortalManager, FortniteGame)
public:

	UPROPERTY(TArray<AFortAthenaCreativePortal*>, AllPortals)
	TArray<AFortAthenaCreativePortal*> AllPortals;

	UPROPERTY(TArray<AFortAthenaCreativePortal*>, UsedPortals)
	TArray<AFortAthenaCreativePortal*> UsedPortals;

	UPROPERTY(TArray<AFortAthenaCreativePortal*>, AvailablePortals)
	TArray<AFortAthenaCreativePortal*> AvailablePortals;

	UPROPERTY(TArray<TWeakObjectPtr<AFortPlayerControllerAthena>>, PendingPortalReservations)
	TArray<TWeakObjectPtr<AFortPlayerControllerAthena>> PendingPortalReservations;

	void AssignPortal(AFortPlayerControllerAthena* PlayerController);
	void LoadPlot(AFortPlayerControllerAthena* PlayerController, UFortCreativeRealEstatePlotItem* Plot);

	void Setup();

	void ReleasePortal(AFortAthenaCreativePortal* PortalToRelease);
	void MarkPortalUsed(AFortAthenaCreativePortal* PortalUsed);
};