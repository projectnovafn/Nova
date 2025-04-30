#pragma once
#include "CoreUObject.h"
#include "Kismet/KismetSystemLibrary.h"

class FServicePermissionsMcp
{
public:
	FString Name;
	FString Id;
	FString Key;
};

class FServiceConfigMcp
{
public:
    inline TArray<FServicePermissionsMcp>& InternalGetServicePermissions()
    {
		return *(TArray<FServicePermissionsMcp>*)(__int64(this) + (UKismetSystemLibrary::GetEngineVersionNum() >= 4.25 ? 0xA8 : 0x98));
    }

    __declspec(property(get = InternalGetServicePermissions))
	TArray<FServicePermissionsMcp> ServicePermissions;

	FServicePermissionsMcp* GetServicePermissionsById(FString Id);
};