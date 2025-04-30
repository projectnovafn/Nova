#include "ItemDefinitions/FortCreativeRealEstatePlotItem.h"

void UFortCreativeRealEstatePlotItem::ParseAttributes(FMcpItem* McpItem)
{
	static auto ParseAttributes = UKismetMemoryLibrary::Get<void (*)(UFortCreativeRealEstatePlotItem*, FMcpItem*)>(L"UFortCreativeRealEstatePlotItem::ParseAttributes");

	ParseAttributes(this, McpItem);
}