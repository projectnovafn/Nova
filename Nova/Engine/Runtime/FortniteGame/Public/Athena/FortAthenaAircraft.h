#pragma once
#include "FortAircraft.h"

class FAircraftFlightInfo
{
	GENERATED_USTRUCT_BODY(FAircraftFlightInfo, FortniteGame)
public:
	
	UPROPERTY_STRUCT(FVector, FlightStartLocation)
	FVector FlightStartLocation;

	UPROPERTY_STRUCT(FRotator, FlightStartRotation)
	FRotator FlightStartRotation;

	UPROPERTY_STRUCT(float, FlightSpeed)
	float FlightSpeed;

	UPROPERTY_STRUCT(float, TimeTillFlightEnd)
	float TimeTillFlightEnd;

	UPROPERTY_STRUCT(float, TimeTillDropStart)
	float TimeTillDropStart;

	UPROPERTY_STRUCT(float, TimeTillDropEnd)
	float TimeTillDropEnd;
};

class AFortAthenaAircraft : public AFortAircraft
{
	GENERATED_UCLASS_BODY(AFortAthenaAircraft, FortniteGame)
public:

	UPROPERTY(FAircraftFlightInfo, FlightInfo)
	FAircraftFlightInfo FlightInfo;
};