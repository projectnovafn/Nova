#include "Athena/Online/Mcp/FortMcpProfileCreative.h"

#include "ItemDefinitions/FortCreativeRealEstatePlotItem.h"

UFortCreativeRealEstatePlotItem* UFortMcpProfileCreative::FindPlotById(FString PlotId)
{
    if (!State) return NULL;

    for (auto& Item : State->ItemsContainer.Items)
    {
        if (Item.Value->InstanceId == PlotId)
            return Cast<UFortCreativeRealEstatePlotItem>(Item.Value->Instance);
    }

    return NULL;
}