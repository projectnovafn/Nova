#include "Patches/McpPatch.h"
#include "KismetDetoursLibrary.h"
#include <vendor\memcury.h>

static void (*_SendRequestNow)(void*, void*, ENetMode);
static void (*_HandleProfileUpdate)(UMcpProfile*, void*, void*);
static bool (*_AutoLogin)(FOnlineIdentityMcp*, int32);
static FServicePermissionsMcp* (*_GetServicePermissionsById)(FServiceConfigMcp*, FString&);
static bool (*_ProcessRequestAsUser)(FOnlineIdentityMcp*, void*, void*, void*, FString&);

UDetour* DispatchRequestDetour = NULL;
UDetour* HandleProfileUpdateDetour = NULL;
UDetour* AutoLoginDetour = NULL;
UDetour* GetServicePermissionsByIdDetour = NULL;
UDetour* ProcessRequestAsUserDetour = NULL;
	
void McpPatch::SendRequestNowHook(UMcpProfileGroup* McpProfile, void* HttpRequest, ENetMode)
{
	auto NetModeOffset = UKismetMemoryLibrary::Get(L"IHttpRequest::NetMode");

	if (NetModeOffset != -1)
		*(DWORD*)(__int64(HttpRequest) + NetModeOffset) = (DWORD)ENetMode::NM_Client;

	_SendRequestNow(McpProfile, HttpRequest, ENetMode::NM_Client);
}

void McpPatch::HandleProfileUpdateHook(UMcpProfile* Profile, void* a2, void* a3)
{
	_HandleProfileUpdate(Profile, a2, a3);

	UE_LOG(LogMCP, Log, TEXT("Updated Profile: %s"), *Profile->ProfileId);

	Profile->HandleProfileUpdate();
}

bool McpPatch::AutoLoginHook(FOnlineIdentityMcp* OnlineIdentity, int32 LocalUserNum)
{
	return OnlineIdentity->AutoLogin(LocalUserNum);
}

FServicePermissionsMcp* McpPatch::GetServicePermissionsByIdHook(FServiceConfigMcp* Config, FString& Id)
{
	return Config->GetServicePermissionsById(Id);
}

bool McpPatch::ProcessRequestAsUserHook(FOnlineSubsystemMcp* OnlineSubsystem, void* McpService, void* UserId, void* HttpRequest, FString& Error)
{
	return OnlineSubsystem->ProcessRequestAsUser(McpService, UserId, HttpRequest, Error);
}

void McpPatch::PostInit()
{
	_HandleProfileUpdate = UKismetMemoryLibrary::Get<decltype(_HandleProfileUpdate)>(L"UMcpProfile::HandleProfileUpdate");

	HandleProfileUpdateDetour = new UDetour();
	HandleProfileUpdateDetour->Init(_HandleProfileUpdate, HandleProfileUpdateHook);
	HandleProfileUpdateDetour->Commit();
}

void McpPatch::Init()
{
	_SendRequestNow = UKismetMemoryLibrary::Get<decltype(_SendRequestNow)>(L"UMcpProfileGroup::SendRequestNow");
	_AutoLogin = UKismetMemoryLibrary::Get<decltype(_AutoLogin)>(L"FOnlineIdentityMcp::AutoLogin");
	_GetServicePermissionsById = UKismetMemoryLibrary::Get<decltype(_GetServicePermissionsById)>(L"FServiceConfigMcp::GetServicePermissionsById");
	_ProcessRequestAsUser = UKismetMemoryLibrary::Get<decltype(_ProcessRequestAsUser)>(L"FOnlineSubsystemMcp::ProcessRequestAsUser");

	DispatchRequestDetour = new UDetour();
	DispatchRequestDetour->Init(_SendRequestNow, SendRequestNowHook);
	DispatchRequestDetour->Commit();

	AutoLoginDetour = new UDetour();
	AutoLoginDetour->Init(_AutoLogin, AutoLoginHook);
	//AutoLoginDetour->Commit();

	GetServicePermissionsByIdDetour = new UDetour();
	GetServicePermissionsByIdDetour->Init(_GetServicePermissionsById, GetServicePermissionsByIdHook);
	GetServicePermissionsByIdDetour->Commit();

	ProcessRequestAsUserDetour = new UDetour();
	ProcessRequestAsUserDetour->Init(_ProcessRequestAsUser, ProcessRequestAsUserHook);
	ProcessRequestAsUserDetour->Commit();

	auto ClientIdRef = Memcury::Scanner::FindStringRef(L"ec684b8c687f479fadea3cb2ad83f5c6").RelativeOffset(3).GetAs<wchar_t*>();
	auto ClientSecretRef = Memcury::Scanner::FindStringRef(L"e1f31c211f28413186262d37a13fc84d").RelativeOffset(3).GetAs<wchar_t*>();

	lstrcpyW(ClientIdRef, FOnlineIdentityMcp::GetClientId().CStr());
	lstrcpyW(ClientSecretRef, FOnlineIdentityMcp::GetClientSecret().CStr());
}