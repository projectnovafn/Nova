#include "Building/BuildingFoundation.h"

void ABuildingFoundation::OnRep_ServerStreamedInLevel()
{
	this->ProcessEvent(L"OnRep_ServerStreamedInLevel");
}

void ABuildingFoundation::OnRep_DynamicFoundationRepData()
{
	this->ProcessEvent(L"OnRep_DynamicFoundationRepData");
}

bool ABuildingFoundation::IsShown()
{
	static auto FoundationEnabledStateProp = this->GetClassProperty(L"FoundationEnabledState");
	static auto DynamicFoundationTypeProp = this->GetClassProperty(L"DynamicFoundationType");

	return (FoundationEnabledStateProp != NULL ? this->FoundationEnabledState == 1 : true) &&
		(DynamicFoundationTypeProp != NULL ? this->DynamicFoundationType  == EDynamicFoundationType::Static : true) &&
		this->bServerStreamedInLevel;
}

void ABuildingFoundation::Show()
{
	static auto FoundationEnabledStateProp = this->GetClassProperty(L"FoundationEnabledState");
	static auto DynamicFoundationTypeProp = this->GetClassProperty(L"DynamicFoundationType");
	static auto DynamicFoundationRepDataProp = this->GetClassProperty(L"DynamicFoundationRepData");

	if (DynamicFoundationTypeProp == NULL)
		return;
	
	this->DynamicFoundationType = EDynamicFoundationType::Static;
	this->bServerStreamedInLevel = true;
	this->OnRep_ServerStreamedInLevel();

	if (FoundationEnabledStateProp != NULL)
		this->FoundationEnabledState = 1;

	if (DynamicFoundationRepDataProp == NULL)
		return;

	this->DynamicFoundationRepData.EnabledState = 1;
	this->OnRep_DynamicFoundationRepData();
}