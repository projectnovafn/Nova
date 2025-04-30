#pragma once
#include "McpProfileGroup.h"

class UMcpProfile;

class FMcpItem
{
public:
	FString ItemType;
	FString InstanceId;
	FString TemplateId;
	int Quantity;
private:
	unsigned char Pad[0x18];
public:
	TMap<FString, void*> Attributes;
	UObject* Instance;
	int64 LastUpdate;
	TWeakObjectPtr<UMcpProfile> OwningProfile;
};

class FMcpItemsContainer
{
public:
	TMap<FString, TSharedPtr<FMcpItem>> Items;
	TMap<FString, FString> ItemsByType;
};

class FMcpProfileState
{
	void* Pad;
public:
	FMcpItemsContainer ItemsContainer;
	TArray<void*> PendingForceQueryDelegates;
	TArray<void*> PendingUpdateQueue;
	int64 MtxBalanceCacheVersion;
	int32 MtxBalanceCache;
};

class UMcpProfile : public UObject
{
	GENERATED_UCLASS_BODY(UMcpProfile, McpProfileSys)
public:

	UPROPERTY(UMcpProfileGroup*, ProfileGroup)
	UMcpProfileGroup* ProfileGroup;

	UPROPERTY(FString, ProfileId)
	FString ProfileId;

	inline FMcpProfileState* InternalGetState()
	{
		static auto StateOffset = GetClassPropertyOffset(L"CommandRevision") + 0x8;
		return *(FMcpProfileState**)(__int64(this) + StateOffset);
	}

	__declspec(property(get = InternalGetState))
	FMcpProfileState* State;

	void HandleProfileUpdate();
};