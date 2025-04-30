#pragma once
#include "LevelSaveComponent.h"

#include "GameFramework/PlayerState.h"
#include "Internationalization/Text.h"
#include "ItemDefinitions/FortPlaysetItemDefinition.h"

class FLocalizedStringPair
{
public:
	FString Locale;
	FString TextLiteral;
};

class FCreativeLoadedLinkData
{
	GENERATED_USTRUCT_BODY(FCreativeLoadedLinkData, FortniteGame)
public:

	UPROPERTY_STRUCT(FString, CreatorName)
	FString CreatorName;

	UPROPERTY_STRUCT(FString, SupportCode)
	FString SupportCode;

	UPROPERTY_STRUCT(FString, Mnemonic)
	FString Mnemonic;

	UPROPERTY_STRUCT(int, Version)
	int Version;

	UPROPERTY_STRUCT(TArray<FLocalizedStringPair>, LinkTitle)
	TArray<FLocalizedStringPair> LinkTitle;

	UPROPERTY_STRUCT(FText, AltTitle)
	FText AltTitle;

	UPROPERTY_STRUCT(FString, LinkYoutubeId)
	FString LinkYoutubeId;

	UPROPERTY_STRUCT(FString, ImageUrl)
	FString ImageUrl;

	UPROPERTY_STRUCT(FString, IslandType)
	FString IslandType;

	UPROPERTY_STRUCT(FString, QuestContextTag)
	FString QuestContextTag;

	UPROPERTY_STRUCT(FString, AccountId)
	FString AccountId;
};

class UFortLevelSaveComponent : public ULevelSaveComponent
{
	GENERATED_UCLASS_BODY(UFortLevelSaveComponent, FortniteGame)
public:

	UPROPERTY(UObject*, LoadedPlot)
	UObject* LoadedPlot;

	UPROPERTY_PTR(FUniqueNetIdRepl, AccountIdOfOwner)
	FUniqueNetIdRepl* AccountIdOfOwner;

	UPROPERTY(FCreativeLoadedLinkData, LoadedLinkData)
	FCreativeLoadedLinkData LoadedLinkData;

	UPROPERTY_BOOL(bIsLoaded)
	bool bIsLoaded;

	UObject* GetSidecar();

	void ResetLevelRecord();
	void LoadFromDssAsync(bool bCheckoutRequired);
	void* LoadPlaysetAsync(TSoftObjectPtr<UFortPlaysetItemDefinition> SoftPlaysetDefinition);
};