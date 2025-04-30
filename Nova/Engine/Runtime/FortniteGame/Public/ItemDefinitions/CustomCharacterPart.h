#pragma once
#include "Engine/DataAsset.h"

enum class EFortCustomGender : uint8
{
	Invalid = 0,
	Male = 1,
	Female = 2,
	Both = 3
};

enum class EFortCustomBodyType : uint8
{
	Small = 0,
	Medium = 1,
	MediumAndSmall = 2,
	Large = 3,
	LargeAndSmall = 4,
	LargeAndMedium = 5,
	All = 6,
	Deprecated = 7
};

enum class EFortCustomPartType : uint8
{
	Head = 0,
	Body = 1,
	Hat = 2,
	Backpack = 3,
	Charm = 4,
	Face = 5,
	NumTypes = 6
};

class UCustomCharacterPart : public UPrimaryDataAsset
{
	GENERATED_UCLASS_BODY(UCustomCharacterPart, FortniteGame)
public:

	UPROPERTY(EFortCustomGender, GenderPermitted)
	EFortCustomGender GenderPermitted;

	UPROPERTY(EFortCustomBodyType, BodyTypesPermitted)
	EFortCustomBodyType BodyTypesPermitted;

	UPROPERTY(EFortCustomPartType, CharacterPartType)
	EFortCustomPartType CharacterPartType;
};