#pragma once
#include "GameFramework/Actor.h"

class ABrush : public AActor
{
	GENERATED_UCLASS_BODY(ABrush, Engine)
public:

	UPROPERTY(UObject*, BrushComponent)
	UObject* BrushComponent;
};