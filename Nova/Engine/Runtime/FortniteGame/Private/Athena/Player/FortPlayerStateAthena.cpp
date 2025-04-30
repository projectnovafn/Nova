#include "Athena/Player/FortPlayerStateAthena.h"
#include "Player/FortPlayerController.h"

void AFortPlayerStateAthena::OnRep_MatchAbandonState(EMatchAbandonState PrevMatchAbandonState)
{
	this->ProcessEvent(L"OnRep_MatchAbandonState", &PrevMatchAbandonState);
}

void AFortPlayerStateAthena::OnRep_TeamKillScore()
{
	this->ProcessEvent(L"OnRep_TeamKillScore");
}

void AFortPlayerStateAthena::OnRep_Kills()
{
	this->ProcessEvent(L"OnRep_Kills");
}

void AFortPlayerStateAthena::OnRep_Place()
{
	this->ProcessEvent(L"OnRep_Place");
}

void AFortPlayerStateAthena::OnRep_TeamScore()
{
	this->ProcessEvent(L"OnRep_TeamScore");
}

void AFortPlayerStateAthena::OnRep_TeamScorePlacement()
{
	this->ProcessEvent(L"OnRep_TeamScorePlacement");
}

void AFortPlayerStateAthena::OnRep_SquadId()
{
	this->ProcessEvent(L"OnRep_SquadId");
}

void AFortPlayerStateAthena::OnRep_DeathInfo()
{
	this->ProcessEvent(L"OnRep_DeathInfo");
}

void AFortPlayerStateAthena::OnRep_StormSurgeEffectCount()
{
	this->ProcessEvent(L"OnRep_StormSurgeEffectCount");
}

void AFortPlayerStateAthena::OnRep_SpectatingTarget()
{
	this->ProcessEvent(L"OnRep_SpectatingTarget");
}

void AFortPlayerStateAthena::ServerSetInAircraft(bool bNewInAircraft)
{

}

void AFortPlayerStateAthena::ClientReportTeamKill(int TeamKills)
{
	this->ProcessEvent(L"ClientReportTeamKill", &TeamKills);
}

void AFortPlayerStateAthena::ClientReportKill(AFortPlayerStateAthena* Player)
{
	this->ProcessEvent(L"ClientReportKill", &Player);
}

void AFortPlayerStateAthena::ClientReportDBNO(AFortPlayerStateAthena* Player)
{
	this->ProcessEvent(L"ClientReportDBNO", &Player);
}

void AFortPlayerStateAthena::ClientNotifyMatchEntered(FString EventId, FString EventWindowId)
{
	struct
	{
		FString EventId;
		FString EventWindowId;
	} params(EventId, EventWindowId);

	this->ProcessEvent(L"ClientNotifyMatchEntered", &params);
}

uint8 AFortPlayerStateAthena::ToDeathCause(FGameplayTagContainer& DeathTags, bool bWasDBNO)
{
	if (GetDefaultObject<AFortPlayerStateAthena>()->FindFunction(TEXT("ToDeathCause")))
	{
		struct
		{
			FGameplayTagContainer TagContainer;
			bool bWasDBNO;
			uint8 ReturnValue;
		}params(DeathTags, bWasDBNO);

		GetDefaultObject<AFortPlayerStateAthena>()->ProcessEvent(L"ToDeathCause", &params);

		return params.ReturnValue;
	}
	else
	{
		static auto ToDeathCause = UKismetMemoryLibrary::Get<uint8(*)(FGameplayTagContainer, bool)>(L"AFortPlayerStateAthena::ToDeathCause");

		if (!ToDeathCause)
			return 0;

		return ToDeathCause(DeathTags, bWasDBNO);
	}
}