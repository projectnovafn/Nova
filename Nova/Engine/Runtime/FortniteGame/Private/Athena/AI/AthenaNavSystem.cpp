#pragma once
#include "Athena/AI/AthenaNavSystem.h"

void UAthenaNavSystem::Build()
{
	static auto Build = UKismetMemoryLibrary::Get<void (*)(UAthenaNavSystem*)>(L"UAthenaNavSystem::Build");

	Build(this);
}