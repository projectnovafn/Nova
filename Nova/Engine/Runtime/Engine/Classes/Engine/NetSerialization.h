#pragma once
#include "CoreUObject.h"

enum class EFastArraySerializerDeltaFlags : uint8
{
	None = 0,
	HasBeenSerialized = 1,
	HasDeltaBeenRequested = 2,
	IsUsingDeltaSerialization = 3
};

class FFastArraySerializerItem
{
public:
	int32 ReplicationID;
	int32 ReplicationKey;
	int32 MostRecentArrayReplicationKey;
};

class FFastArraySerializer
{
	TMap<int32, int32> ItemMap;
	int IDCounter;
	int ArrayReplicationKey;
	unsigned char Pad1[0x50];
	unsigned char Pad2[0x50];
public:

	void MarkItemDirty(FFastArraySerializerItem& Item)
	{
		if (Item.ReplicationID == -1)
		{
			Item.ReplicationID = ++IDCounter;
			if (IDCounter == -1)
			{
				IDCounter++;
			}
		}

		Item.ReplicationKey++;
		MarkArrayDirty();
	}

	void MarkArrayDirty()
	{
		ItemMap.Reset();
		IncrementArrayReplicationKey();

		CachedNumItems = -1;
		CachedNumItemsToConsiderForWriting = -1;
	}

	void IncrementArrayReplicationKey()
	{
		ArrayReplicationKey++;
		if (ArrayReplicationKey == -1)
		{
			ArrayReplicationKey++;
		}
	}

	int CachedNumItems;
	int CachedNumItemsToConsiderForWriting;
	EFastArraySerializerDeltaFlags DeltaFlags;
};