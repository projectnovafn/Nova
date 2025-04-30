#pragma once
#include "Engine.h"
#include "Inventory/FortPickupAthena.h"
#include "Player/FortPlayerController.h"
#include "Kismet/KismetMathLibrary.h"
#include "ItemDefinitions/FortWeaponItemDefinition.h"
#include "Projectiles/FortProjectileBase.h"
#include "Building/BuildingItemCollectorActor.h"

class AFortPlayerState;

class FFortLootTierData
{
    GENERATED_USTRUCT_BODY(FFortLootTierData, FortniteGame)

public:

    UPROPERTY_STRUCT(FName, TierGroup)
    FName TierGroup;

    UPROPERTY_STRUCT(float, Weight)
    float Weight;

    UPROPERTY_STRUCT(uint8_t, QuotaLevel)
    uint8_t QuotaLevel;

    UPROPERTY_STRUCT(int, LootTier)
    int LootTier;

    UPROPERTY_STRUCT(int, MinWorldLevel)
    int MinWorldLevel;

    UPROPERTY_STRUCT(int, MaxWorldLevel)
    int MaxWorldLevel;

    UPROPERTY_STRUCT(FString, StreakBreakerCurrency)
    FString StreakBreakerCurrency;

    UPROPERTY_STRUCT(int, StreakBreakerPointsMin)
    int StreakBreakerPointsMin;

    UPROPERTY_STRUCT(int, StreakBreakerPointsMax)
    int StreakBreakerPointsMax;

    UPROPERTY_STRUCT(int, StreakBreakerPointsSpend)
    int StreakBreakerPointsSpend;

    UPROPERTY_STRUCT(FName, LootPackage)
    FName LootPackage;

    UPROPERTY_STRUCT(FName, LootPreviewPackage)
    FName LootPreviewPackage;

    UPROPERTY_STRUCT(float, NumLootPackageDrops)
    float NumLootPackageDrops;

    UPROPERTY_STRUCT(TArray<int>, LootPackageCategoryWeightArray)
    TArray<int> LootPackageCategoryWeightArray;

    UPROPERTY_STRUCT(TArray<int>, LootPackageCategoryMinArray)
    TArray<int> LootPackageCategoryMinArray;

    UPROPERTY_STRUCT(TArray<int>, LootPackageCategoryMaxArray)
    TArray<int> LootPackageCategoryMaxArray;

    UPROPERTY_STRUCT(FGameplayTagContainer, GameplayTags)
    FGameplayTagContainer GameplayTags;

    UPROPERTY_STRUCT(FGameplayTagContainer, RequiredGameplayTags)
    FGameplayTagContainer RequiredGameplayTags;

    UPROPERTY_STRUCT(bool, bAllowBonusLootDrops)
    bool bAllowBonusLootDrops;

    UPROPERTY_STRUCT(FString, Annotation)
    FString Annotation;
};

class FFortLootPackageData
{
    GENERATED_USTRUCT_BODY(FFortLootPackageData, FortniteGame)

public:

    UPROPERTY_STRUCT(FName, LootPackageID)
    FName LootPackageID;

    UPROPERTY_STRUCT(float, Weight)
    float Weight;

    UPROPERTY_STRUCT(FName, NamedWeightMult)
    FName NamedWeightMult;

    UPROPERTY_STRUCT(int, Count)
    int Count;

    UPROPERTY_STRUCT(int, LootPackageCategory)
    int LootPackageCategory;

    UPROPERTY_STRUCT(FGameplayTagContainer, GameplayTags)
    FGameplayTagContainer GameplayTags;

    UPROPERTY_STRUCT(FName, RequiredTag)
    FName RequiredTag;

    UPROPERTY_STRUCT(FString, LootPackageCall)
    FString LootPackageCall;

    UPROPERTY_STRUCT(TSoftObjectPtr<UFortItemDefinition>, ItemDefinition)
    TSoftObjectPtr<UFortItemDefinition> ItemDefinition;

    UPROPERTY_STRUCT(FString, PersistentLevel)
    FString PersistentLevel;

    UPROPERTY_STRUCT(int, MinWorldLevel)
    int MinWorldLevel;

    UPROPERTY_STRUCT(int, MaxWorldLevel)
    int MaxWorldLevel;

    UPROPERTY_STRUCT(bool, bAllowBonusDrops)
    bool bAllowBonusDrops;

    UPROPERTY_STRUCT(FString, Annotation)
    FString Annotation;
};

class FFortBaseWeaponStats
{
    GENERATED_USTRUCT_BODY(FFortBaseWeaponStats, FortniteGame)

public:
    UPROPERTY_STRUCT(int, BaseLevel)
    int BaseLevel;

    UPROPERTY_STRUCT(FName, NamedWeightRow)
    FName NamedWeightRow;

    UPROPERTY_STRUCT(float, DmgPB)
    float DmgPB;

    UPROPERTY_STRUCT(float, DmgMid)
    float DmgMid;

    UPROPERTY_STRUCT(float, DmgLong)
    float DmgLong;

    UPROPERTY_STRUCT(float, DmgMaxRange)
    float DmgMaxRange;

    UPROPERTY_STRUCT(float, EnvDmgPB)
    float EnvDmgPB;

    UPROPERTY_STRUCT(float, EnvDmgMid)
    float EnvDmgMid;

    UPROPERTY_STRUCT(float, EnvDmgLong)
    float EnvDmgLong;

    UPROPERTY_STRUCT(float, EnvDmgMaxRange)
    float EnvDmgMaxRange;

    UPROPERTY_STRUCT(float, ImpactDmgPB)
    float ImpactDmgPB;

    UPROPERTY_STRUCT(float, ImpactDmgMid)
    float ImpactDmgMid;

    UPROPERTY_STRUCT(float, ImpactDmgLong)
    float ImpactDmgLong;

    UPROPERTY_STRUCT(float, ImpactDmgMaxRange)
    float ImpactDmgMaxRange;

    UPROPERTY_STRUCT(bool, bForceControl)
    bool bForceControl;

    UPROPERTY_STRUCT(float, RngPB)
    float RngPB;

    UPROPERTY_STRUCT(float, RngMid)
    float RngMid;

    UPROPERTY_STRUCT(float, RngLong)
    float RngLong;

    UPROPERTY_STRUCT(float, RngMax)
    float RngMax;

    UPROPERTY_STRUCT(class UCurveTable*, DmgScaleTable)
    class UCurveTable* DmgScaleTable;

    UPROPERTY_STRUCT(FName, DmgScaleTableRow)
    FName DmgScaleTableRow;

    UPROPERTY_STRUCT(float, DmgScale)
    float DmgScale;

    UPROPERTY_STRUCT(class UCurveTable*, EnvDmgScaleTable)
    class UCurveTable* EnvDmgScaleTable;

    UPROPERTY_STRUCT(FName, EnvDmgScaleTableRow)
    FName EnvDmgScaleTableRow;

    UPROPERTY_STRUCT(float, EnvDmgScale)
    float EnvDmgScale;

    UPROPERTY_STRUCT(class UCurveTable*, ImpactDmgScaleTable)
    class UCurveTable* ImpactDmgScaleTable;

    UPROPERTY_STRUCT(FName, ImpactDmgScaleTableRow)
    FName ImpactDmgScaleTableRow;

    UPROPERTY_STRUCT(float, ImpactDmgScale)
    float ImpactDmgScale;

    UPROPERTY_STRUCT(FName, SurfaceRatioRowName)
    FName SurfaceRatioRowName;

    UPROPERTY_STRUCT(float, DamageZone_Light)
    float DamageZone_Light;

    UPROPERTY_STRUCT(float, DamageZone_Normal)
    float DamageZone_Normal;

    UPROPERTY_STRUCT(float, DamageZone_Critical)
    float DamageZone_Critical;

    UPROPERTY_STRUCT(float, DamageZone_Vulnerability)
    float DamageZone_Vulnerability;

    UPROPERTY_STRUCT(float, KnockbackMagnitude)
    float KnockbackMagnitude;

    UPROPERTY_STRUCT(float, MidRangeKnockbackMagnitude)
    float MidRangeKnockbackMagnitude;

    UPROPERTY_STRUCT(float, LongRangeKnockbackMagnitude)
    float LongRangeKnockbackMagnitude;

    UPROPERTY_STRUCT(float, KnockbackZAngle)
    float KnockbackZAngle;

    UPROPERTY_STRUCT(float, StunTime)
    float StunTime;

    UPROPERTY_STRUCT(float, StunScale)
    float StunScale;

    UPROPERTY_STRUCT(class UDataTable*, Durability)
    class UDataTable* Durability;

    UPROPERTY_STRUCT(FName, DurabilityRowName)
    FName DurabilityRowName;

    UPROPERTY_STRUCT(float, DurabilityScale)
    float DurabilityScale;

    UPROPERTY_STRUCT(float, DurabilityPerUse)
    float DurabilityPerUse;

    UPROPERTY_STRUCT(float, DiceCritChance)
    float DiceCritChance;

    UPROPERTY_STRUCT(float, DiceCritDamageMultiplier)
    float DiceCritDamageMultiplier;

    UPROPERTY_STRUCT(float, ReloadTime)
    float ReloadTime;

    UPROPERTY_STRUCT(float, ReloadScale)
    float ReloadScale;

    UPROPERTY_STRUCT(uint8_t, ReloadType)
    uint8_t ReloadType;

    UPROPERTY_STRUCT(bool, bAllowReloadInterrupt)
    bool bAllowReloadInterrupt;

    UPROPERTY_STRUCT(bool, bReloadInterruptIsImmediate)
    bool bReloadInterruptIsImmediate;

    UPROPERTY_STRUCT(int, ClipSize)
    int ClipSize;

    UPROPERTY_STRUCT(float, ClipScale)
    float ClipScale;

    UPROPERTY_STRUCT(int, InitialClips)
    int InitialClips;

    UPROPERTY_STRUCT(int, CartridgePerFire)
    int CartridgePerFire;

    UPROPERTY_STRUCT(int, AmmoCostPerFire)
    int AmmoCostPerFire;

    UPROPERTY_STRUCT(int, MaxAmmoCostPerFire)
    int MaxAmmoCostPerFire;

    UPROPERTY_STRUCT(float, MinChargeTime)
    float MinChargeTime;

    UPROPERTY_STRUCT(float, MaxChargeTime)
    float MaxChargeTime;

    UPROPERTY_STRUCT(float, ChargeDownTime)
    float ChargeDownTime;

    UPROPERTY_STRUCT(float, MinChargeDamageMultiplier)
    float MinChargeDamageMultiplier;

    UPROPERTY_STRUCT(float, MaxChargeDamageMultiplier)
    float MaxChargeDamageMultiplier;

    UPROPERTY_STRUCT(float, EquipAnimRate)
    float EquipAnimRate;

    UPROPERTY_STRUCT(float, QuickBarSlotCooldownDuration)
    float QuickBarSlotCooldownDuration;
};


class FFortRangedWeaponStats : public FFortBaseWeaponStats
{
    GENERATED_USTRUCT_BODY(FFortRangedWeaponStats, FortniteGame)

public:

    UPROPERTY_STRUCT(float, Spread)
    float Spread;

    UPROPERTY_STRUCT(float, SpreadDownsights)
    float SpreadDownsights;

    UPROPERTY_STRUCT(float, StandingStillSpreadMultiplier)
    float StandingStillSpreadMultiplier;

    UPROPERTY_STRUCT(float, AthenaCrouchingSpreadMultiplier)
    float AthenaCrouchingSpreadMultiplier;

    UPROPERTY_STRUCT(float, AthenaJumpingFallingSpreadMultiplier)
    float AthenaJumpingFallingSpreadMultiplier;

    UPROPERTY_STRUCT(float, AthenaSprintingSpreadMultiplier)
    float AthenaSprintingSpreadMultiplier;

    UPROPERTY_STRUCT(float, MinSpeedForSpreadMultiplier)
    float MinSpeedForSpreadMultiplier;

    UPROPERTY_STRUCT(float, MaxSpeedForSpreadMultiplier)
    float MaxSpeedForSpreadMultiplier;

    UPROPERTY_STRUCT(float, SpreadDownsightsAdditionalCooldownTime)
    float SpreadDownsightsAdditionalCooldownTime;

    UPROPERTY_STRUCT(float, HeatX1)
    float HeatX1;

    UPROPERTY_STRUCT(float, HeatY1)
    float HeatY1;

    UPROPERTY_STRUCT(float, HeatX2)
    float HeatX2;

    UPROPERTY_STRUCT(float, HeatY2)
    float HeatY2;

    UPROPERTY_STRUCT(float, HeatX3)
    float HeatX3;

    UPROPERTY_STRUCT(float, HeatY3)
    float HeatY3;

    UPROPERTY_STRUCT(float, HeatXScale)
    float HeatXScale;

    UPROPERTY_STRUCT(float, HeatYScale)
    float HeatYScale;

    UPROPERTY_STRUCT(float, CoolX1)
    float CoolX1;

    UPROPERTY_STRUCT(float, CoolY1)
    float CoolY1;

    UPROPERTY_STRUCT(float, CoolX2)
    float CoolX2;

    UPROPERTY_STRUCT(float, CoolY2)
    float CoolY2;

    UPROPERTY_STRUCT(float, CoolX3)
    float CoolX3;

    UPROPERTY_STRUCT(float, CoolY3)
    float CoolY3;

    UPROPERTY_STRUCT(float, CoolXScale)
    float CoolXScale;

    UPROPERTY_STRUCT(float, CoolYScale)
    float CoolYScale;

    UPROPERTY_STRUCT(float, PerfectAimCooldown)
    float PerfectAimCooldown;

    UPROPERTY_STRUCT(int, BulletsPerCartridge)
    int BulletsPerCartridge;

    UPROPERTY_STRUCT(float, FiringRate)
    float FiringRate;

    UPROPERTY_STRUCT(float, ROFScale)
    float ROFScale;

    UPROPERTY_STRUCT(float, BurstFiringRate)
    float BurstFiringRate;

    UPROPERTY_STRUCT(float, FiringRateDownsightsMultiplier)
    float FiringRateDownsightsMultiplier;

    UPROPERTY_STRUCT(float, RecoilVert)
    float RecoilVert;

    UPROPERTY_STRUCT(float, RecoilVertScale)
    float RecoilVertScale;

    UPROPERTY_STRUCT(float, RecoilVertScaleGamepad)
    float RecoilVertScaleGamepad;

    UPROPERTY_STRUCT(float, VertRecoilDownChance)
    float VertRecoilDownChance;

    UPROPERTY_STRUCT(float, RecoilHoriz)
    float RecoilHoriz;

    UPROPERTY_STRUCT(float, RecoilHorizScale)
    float RecoilHorizScale;

    UPROPERTY_STRUCT(float, RecoilHorizScaleGamepad)
    float RecoilHorizScaleGamepad;

    UPROPERTY_STRUCT(float, RecoilInterpSpeed)
    float RecoilInterpSpeed;

    UPROPERTY_STRUCT(float, RecoilRecoveryInterpSpeed)
    float RecoilRecoveryInterpSpeed;

    UPROPERTY_STRUCT(float, RecoilRecoveryDelay)
    float RecoilRecoveryDelay;

    UPROPERTY_STRUCT(float, RecoilRecoveryFraction)
    float RecoilRecoveryFraction;

    UPROPERTY_STRUCT(float, RecoilDownsightsMultiplier)
    float RecoilDownsightsMultiplier;

    UPROPERTY_STRUCT(float, AthenaRecoilMagnitudeMin)
    float AthenaRecoilMagnitudeMin;

    UPROPERTY_STRUCT(float, AthenaRecoilMagnitudeMax)
    float AthenaRecoilMagnitudeMax;

    UPROPERTY_STRUCT(float, AthenaRecoilMagnitudeScale)
    float AthenaRecoilMagnitudeScale;

    UPROPERTY_STRUCT(float, AthenaRecoilAngleMin)
    float AthenaRecoilAngleMin;

    UPROPERTY_STRUCT(float, AthenaRecoilAngleMax)
    float AthenaRecoilAngleMax;

    UPROPERTY_STRUCT(float, AthenaRecoilRollMagnitudeMin)
    float AthenaRecoilRollMagnitudeMin;

    UPROPERTY_STRUCT(float, AthenaRecoilRollMagnitudeMax)
    float AthenaRecoilRollMagnitudeMax;

    UPROPERTY_STRUCT(float, AthenaRecoilInterpSpeed)
    float AthenaRecoilInterpSpeed;

    UPROPERTY_STRUCT(float, AthenaRecoilRecoveryInterpSpeed)
    float AthenaRecoilRecoveryInterpSpeed;

    UPROPERTY_STRUCT(float, AthenaRecoilDownsightsMultiplier)
    float AthenaRecoilDownsightsMultiplier;

    UPROPERTY_STRUCT(float, AthenaRecoilHipFireMultiplier)
    float AthenaRecoilHipFireMultiplier;

    UPROPERTY_STRUCT(float, AthenaAimAssistRange)
    float AthenaAimAssistRange;

    UPROPERTY_STRUCT(float, ADSTransitionInTime)
    float ADSTransitionInTime;

    UPROPERTY_STRUCT(float, ADSTransitionOutTime)
    float ADSTransitionOutTime;

    UPROPERTY_STRUCT(int, MaxSpareAmmo)
    int MaxSpareAmmo;

    UPROPERTY_STRUCT(int, BulletsPerTracer)
    int BulletsPerTracer;

    UPROPERTY_STRUCT(float, AIDelayBeforeFiringMin)
    float AIDelayBeforeFiringMin;

    UPROPERTY_STRUCT(float, AIDelayBeforeFiringMax)
    float AIDelayBeforeFiringMax;

    UPROPERTY_STRUCT(float, AIFireDurationMin)
    float AIFireDurationMin;

    UPROPERTY_STRUCT(float, AIFireDurationMax)
    float AIFireDurationMax;

    UPROPERTY_STRUCT(float, AIMinSpreadDuration)
    float AIMinSpreadDuration;

    UPROPERTY_STRUCT(float, AIMaxSpreadDuration)
    float AIMaxSpreadDuration;

    UPROPERTY_STRUCT(float, AIDurationSpreadMultiplier)
    float AIDurationSpreadMultiplier;

    UPROPERTY_STRUCT(float, AIAdditionalSpreadForTargetMovingLaterally)
    float AIAdditionalSpreadForTargetMovingLaterally;

    UPROPERTY_STRUCT(float, AIAthenaHearFiringNoiseRange)
    float AIAthenaHearFiringNoiseRange;

    UPROPERTY_STRUCT(float, EQSDensity)
    float EQSDensity;

    UPROPERTY_STRUCT(float, MinApproachRange)
    float MinApproachRange;

    UPROPERTY_STRUCT(float, MinActualRange)
    float MinActualRange;

    UPROPERTY_STRUCT(float, MinPreferredRange)
    float MinPreferredRange;

    UPROPERTY_STRUCT(float, MinPreferredRangeEQS)
    float MinPreferredRangeEQS;

    UPROPERTY_STRUCT(float, MaxPreferredRangeEQS)
    float MaxPreferredRangeEQS;

    UPROPERTY_STRUCT(float, MaxPreferredRange)
    float MaxPreferredRange;

    UPROPERTY_STRUCT(float, MaxActualRange)
    float MaxActualRange;

    UPROPERTY_STRUCT(float, MaxApproachRange)
    float MaxApproachRange;

    UPROPERTY_STRUCT(float, SweepRadius)
    float SweepRadius;

    UPROPERTY_STRUCT(float, AutoReloadDelayOverride)
    float AutoReloadDelayOverride;

    UPROPERTY_STRUCT(float, OverheatingMaxValue)
    float OverheatingMaxValue;

    UPROPERTY_STRUCT(float, OverheatHeatingValue)
    float OverheatHeatingValue;

    UPROPERTY_STRUCT(float, OverheatingCoolingValue)
    float OverheatingCoolingValue;

    UPROPERTY_STRUCT(float, HeatingCooldownDelay)
    float HeatingCooldownDelay;

    UPROPERTY_STRUCT(float, OverheatedCooldownDelay)
    float OverheatedCooldownDelay;
};

class FFortGameplayEffectContainerSpec
{
private:
    unsigned char Pad[0x88];
};

class UFortKismetLibrary : public UObject
{
	GENERATED_UCLASS_BODY(UFortKismetLibrary, FortniteGame)

public:

	static AActor* GetClosestActorFromArray(AActor* Actor, TArray<AActor*> Actors);

    template <typename T>
    static T* GetClosestActorFromArray(AActor* Actor, TArray<T*> Actors)
    {
        return (T*)GetClosestActorFromArray(Actor, *(TArray<AActor*>*)&Actors);
    }

	static AFortPickupAthena* SpawnPickup(UWorld* World, FVector Location, UFortItemDefinition* ItemDefinition, int Count, int LoadedAmmo = 0, AFortPlayerController* Instigator = NULL, EFortPickupSourceTypeFlag InPickupSourceTypeFlags = EFortPickupSourceTypeFlag::Other, EFortPickupSpawnSource InPickupSpawnSource = EFortPickupSpawnSource::Unset);

    static void GiveItemToInventoryOwner(AFortPlayerController* InventoryOwner, UFortWorldItemDefinition* ItemDefinition, int NumberToGive, bool bNotifyPlayer, int ItemLevel, int PickupInstigatorHandle);

	static UFortItemDefinition* GetResourceItemDefinition(EFortResourceType ResourceType);

	static std::vector<FFortItemEntry*> PickLootDrops(UWorld* World, FName TierGroupName, int ForcedLootTier = -1, int Count = 0);

    static void SpawnLlamas(UWorld* World);
    static void FillVendingMachine(UWorld* World, ABuildingItemCollectorActor* ItemCollector);
    static void SpawnDynamicItems(UWorld* World);
    
    static bool UpdatePlayerCustomCharacterPartsVisualization(AFortPlayerState* PlayerState);

    static AFortProjectileBase* SpawnProjectile(UClass* ProjectileClass, AActor* RequestedBy, FVector SpawnLocation, FRotator SpawnRotation, FRotator SpawnDirection, FFortGameplayEffectContainerSpec EffectContainerSpecToApplyOnHit, FFortGameplayEffectContainerSpec EffectContainerSpecToApplyOnExplode, bool bUseDefaultPhysics, AActor* HomingTarget, float InitialSpeed, float GravityScale, float ChargePercent);

    static int32 GetClipSize(UFortWeaponItemDefinition* WeaponDef);

    static TArray<UObject*> JonLHack_GetAllObjectsOfClassFromPath(FString Path, UClass* Class);

    static FVector FindStaticGroundLocationAt(UWorld* World, FVector InLocation, AActor* IgnoreActor, float TraceStartZ, float TraceEndZ);
};