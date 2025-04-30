#include "ServiceConfigMcp.h"

FServicePermissionsMcp* FServiceConfigMcp::GetServicePermissionsById(FString Id)
{
	return &this->ServicePermissions[0];
}