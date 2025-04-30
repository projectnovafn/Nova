#include "ItemDefinitions/FortPlaysetItemDefinition.h"

#include "Kismet/FortKismetLibrary.h"

void UFortPlaysetItemDefinition::Setup()
{
	auto Objects = UFortKismetLibrary::JonLHack_GetAllObjectsOfClassFromPath(L"/Game/Playsets", UObject::StaticClass());

	for (auto Object : Objects)
	{
		auto Playset = Cast<UFortPlaysetItemDefinition>(Object);

		if (Playset && Playset->LevelSaveRecord)
		{
			auto FoundPlayset = Cast<UFortPlaysetItemDefinition>(StaticFindObject(NULL, ANY_PACKAGE, *Object->GetName(), false));

			if (FoundPlayset)
				FoundPlayset->LevelSaveRecord = DuplicateObject(Playset->LevelSaveRecord, FoundPlayset);
		}
	}
}
