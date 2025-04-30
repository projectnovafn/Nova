#pragma once
#include "Online/Mcp/FortMcpProfile.h"

class UFortCreativeRealEstatePlotItem;

class UFortMcpProfileCreative : public UFortMcpProfile
{
	GENERATED_UCLASS_BODY(UFortMcpProfileCreative, FortniteGame)
public:

	UFortCreativeRealEstatePlotItem* FindPlotById(FString PlotId);
};