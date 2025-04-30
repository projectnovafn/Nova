#include "Inventory/FortWorldItem.h"

void UFortWorldItem::SetOwner(AFortPlayerController* PlayerController)
{
	this->ProcessEvent(L"SetOwningControllerForTemporaryItem", &PlayerController);
}