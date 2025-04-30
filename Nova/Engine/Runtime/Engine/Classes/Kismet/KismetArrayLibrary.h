#pragma once
#include "CoreUObject.h"

class UKismetArrayLibrary : public UObject
{
	GENERATED_UCLASS_BODY(UKismetArrayLibrary, Engine)
public:

	template <typename ElementType>
	static int32 Array_Add(TArray<ElementType>& TargetArray, PropertyWrapper* ArrayProp, const ElementType& Item)
	{
		static auto Array_Add = UKismetMemoryLibrary::Get<int32(*)(void*, void*, void*)>(L"UKismetArrayLibrary::Array_Add");

		return Array_Add(&TargetArray, ArrayProp->UnderlyingObject, (void*)&Item);
	}

	template <typename ElementType>
	static int32 Array_Remove(TArray<ElementType>& TargetArray, PropertyWrapper* ArrayProp, int32 IndexToRemove)
	{
		static auto Array_Remove = UKismetMemoryLibrary::Get<int32(*)(void*, void*, int32)>(L"UKismetArrayLibrary::Array_Remove");

		return Array_Remove(&TargetArray, ArrayProp->UnderlyingObject, IndexToRemove);
	}

	template <typename ElementType>
	static int32 Array_RemoveItem(TArray<ElementType>& TargetArray, PropertyWrapper* ArrayProp, ElementType& Item)
	{
		for (int i = 0; i < TargetArray.Num(); i++)
		{
			if (TargetArray[i] != Item)
				continue;

			UKismetArrayLibrary::Array_Remove(TargetArray, ArrayProp, i);
			return i;
		}

		return -1;
	}
};