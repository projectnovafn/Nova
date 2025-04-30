#include "Player/FortPlayerState.h"
#include "Kismet/FortKismetLibrary.h"

UCustomCharacterPart** AFortPlayerState::GetCharacterParts()
{
	return GetClassField(L"CharacterParts") ? (FCustomCharacterParts::StaticStruct() ? &CharacterParts.Parts : (UCustomCharacterPart**)&CharacterParts) : &CharacterData.Parts;
}

void AFortPlayerState::ApplyCharacterCustomization(AFortPlayerPawn* Pawn)
{
	UE_LOG(LogFort, Log, TEXT("ApplyCharacterCustomization: %s"), *Pawn->GetName());

	if (auto NativeFunc = UKismetMemoryLibrary::Get<void(*)(AFortPlayerState*, AFortPlayerPawn*)>(L"AFortPlayerState::ApplyCharacterCustomization"))
	{
		return NativeFunc(this, Pawn);
	}

	auto PlayerController = Cast<AFortPlayerController>(GetOwner());

	if (!PlayerController)
		return;

	auto& CosmeticLoadout = PlayerController->GetCosmeticLoadout();

	if (CosmeticLoadout.Character != NULL)
	{
		auto CharacterParts = CosmeticLoadout.Character->HeroDefinition->Specializations[0]->CharacterParts;

		HeroType = CosmeticLoadout.Character->HeroDefinition;
		CharacterGender = CharacterParts[0]->GenderPermitted;
		CharacterBodyType = CharacterParts[0]->BodyTypesPermitted;

		for (auto& CharacterPart : CharacterParts)
		{
			GetCharacterParts()[(int)CharacterPart->CharacterPartType] = CharacterPart.Get();
		}
	}

	if (CosmeticLoadout.Backpack != NULL)
	{
		for (auto& CharacterPart : CosmeticLoadout.Backpack->CharacterParts)
		{
			GetCharacterParts()[(int)CharacterPart->CharacterPartType] = CharacterPart;
		}
	}

	UFortKismetLibrary::UpdatePlayerCustomCharacterPartsVisualization(this);
}

bool AFortPlayerState::AreUniqueIDsIdentical(FUniqueNetIdRepl& A, FUniqueNetIdRepl& B)
{
	struct
	{
		FUniqueNetIdRepl A;
		FUniqueNetIdRepl B;
		bool ReturnValue;
	} params(A, B);

	GetDefaultObject<AFortPlayerState>()->ProcessEvent(L"AreUniqueIDsIdentical", &params);

	return params.ReturnValue;
}