#pragma once
#include "GameFramework/PlayerState.h"
#include "Pawns/FortPlayerPawn.h"
#include "Engine/DataAsset.h"

#include "ItemDefinitions/FortHeroType.h"

class UAbilitySystemComponent;

class UFortAbilitySet : public UPrimaryDataAsset
{
	GENERATED_UCLASS_BODY(UFortAbilitySet, FortniteGame)
public:

	UPROPERTY(TArray<UClass*>, GameplayAbilities)
	TArray<UClass*> GameplayAbilities;
};

class FCustomCharacterData
{
	GENERATED_USTRUCT_BODY(FCustomCharacterData, FortniteGame)
public:

	UPROPERTY_STRUCT(UCustomCharacterPart*, Parts)
	UCustomCharacterPart* Parts;
};

class FCustomCharacterParts
{
	GENERATED_USTRUCT_BODY(FCustomCharacterParts, FortniteGame)
public:

	UPROPERTY_STRUCT(UCustomCharacterPart*, Parts)
	UCustomCharacterPart* Parts;
};

class AFortPlayerState : public APlayerState
{
	GENERATED_UCLASS_BODY(AFortPlayerState, FortniteGame)
public:


	UPROPERTY_BOOL_NOTIFY(bHasStartedPlaying)
	bool bHasStartedPlaying;

	UPROPERTY_BOOL(bHasFinishedLoading)
	bool bHasFinishedLoading;

	UPROPERTY(UAbilitySystemComponent*, AbilitySystemComponent)
	UAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY_NOTIFY(UFortHeroType*, HeroType)
	UFortHeroType* HeroType;

	UPROPERTY(EFortCustomGender, CharacterGender)
	EFortCustomGender CharacterGender;

	UPROPERTY(EFortCustomBodyType, CharacterBodyType)
	EFortCustomBodyType CharacterBodyType;

	UPROPERTY(FCustomCharacterParts, CharacterParts)
	FCustomCharacterParts CharacterParts;

	UPROPERTY(FCustomCharacterData, CharacterData)
	FCustomCharacterData CharacterData;

	UPROPERTY(int, WorldPlayerId)
	int WorldPlayerId;

	UCustomCharacterPart** GetCharacterParts();

	void ApplyCharacterCustomization(AFortPlayerPawn* Pawn);

	static bool AreUniqueIDsIdentical(FUniqueNetIdRepl& A, FUniqueNetIdRepl& B);
};