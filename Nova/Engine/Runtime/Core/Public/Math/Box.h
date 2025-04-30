// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreTypes.h"
#include "Misc/AssertionMacros.h"
#include "Containers/UnrealString.h"
#include "Math/Vector.h"

/**
 * Implements an axis-aligned box.
 *
 * Boxes describe an axis-aligned extent in three dimensions. They are used for many different things in the
 * Engine and in games, such as bounding volumes, collision detection and visibility calculation.
 */
struct FBox
{
public:

	/** Holds the box's minimum point. */
	FVector Min;

	/** Holds the box's maximum point. */
	FVector Max;

	/** Holds a flag indicating whether this box is valid. */
	uint8 IsValid;

public:

	/** Default constructor (no initialization). */
	FBox() {}

	/**
	 * Creates and initializes a new box with zero extent and marks it as invalid.
	 *
	 * @param EForceInit Force Init Enum.
	 */
	explicit FBox(EForceInit)
	{
		Init();
	}

	/**
	 * Creates and initializes a new box from the specified extents.
	 *
	 * @param InMin The box's minimum point.
	 * @param InMax The box's maximum point.
	 */
	FBox(const FVector& InMin, const FVector& InMax)
		: Min(InMin)
		, Max(InMax)
		, IsValid(1)
	{
	}

public:

	/**
	 * Compares two boxes for equality.
	 *
	 * @return true if the boxes are equal, false otherwise.
	 */
	FORCEINLINE bool operator==(const FBox& Other) const
	{
		return (Min == Other.Min) && (Max == Other.Max);
	}

	/**
	 * Gets reference to the min or max of this bounding volume.
	 *
	 * @param Index the index into points of the bounding volume.
	 * @return a reference to a point of the bounding volume.
	 */
	FORCEINLINE FVector& operator[](int32 Index)
	{
		check((Index >= 0) && (Index < 2));

		if (Index == 0)
		{
			return Min;
		}

		return Max;
	}

public:

	/**
	 * Calculates the distance of a point to this box.
	 *
	 * @param Point The point.
	 * @return The distance.
	 */
	FORCEINLINE float ComputeSquaredDistanceToPoint(const FVector& Point) const
	{
		return ComputeSquaredDistanceFromBoxToPoint(Min, Max, Point);
	}

	/**
	 * Increases the box size.
	 *
	 * @param W The size to increase the volume by.
	 * @return A new bounding box.
	 */
	FORCEINLINE FBox ExpandBy(float W) const
	{
		return FBox(Min - FVector(W, W, W), Max + FVector(W, W, W));
	}

	/**
	* Increases the box size.
	*
	* @param V The size to increase the volume by.
	* @return A new bounding box.
	*/
	FORCEINLINE FBox ExpandBy(const FVector& V) const
	{
		return FBox(Min - V, Max + V);
	}

	/**
	* Increases the box size.
	*
	* @param Neg The size to increase the volume by in the negative direction (positive values move the bounds outwards)
	* @param Pos The size to increase the volume by in the positive direction (positive values move the bounds outwards)
	* @return A new bounding box.
	*/
	FBox ExpandBy(const FVector& Neg, const FVector& Pos) const
	{
		return FBox(Min - Neg, Max + Pos);
	}

	/**
	 * Shifts the bounding box position.
	 *
	 * @param Offset The vector to shift the box by.
	 * @return A new bounding box.
	 */
	FORCEINLINE FBox ShiftBy(const FVector& Offset) const
	{
		return FBox(Min + Offset, Max + Offset);
	}

	/**
	 * Moves the center of bounding box to new destination.
	 *
	 * @param The destination point to move center of box to.
	 * @return A new bounding box.
	 */
	FORCEINLINE FBox MoveTo(const FVector& Destination) const
	{
		const FVector Offset = Destination - GetCenter();
		return FBox(Min + Offset, Max + Offset);
	}

	/**
	 * Gets the center point of this box.
	 *
	 * @return The center point.
	 * @see GetCenterAndExtents, GetExtent, GetSize, GetVolume
	 */
	FORCEINLINE FVector GetCenter() const
	{
		return FVector((Min + Max) * 0.5f);
	}

	/**
	 * Gets the center and extents of this box.
	 *
	 * @param center[out] Will contain the box center point.
	 * @param Extents[out] Will contain the extent around the center.
	 * @see GetCenter, GetExtent, GetSize, GetVolume
	 */
	FORCEINLINE void GetCenterAndExtents(FVector& center, FVector& Extents) const
	{
		Extents = GetExtent();
		center = Min + Extents;
	}

	/**
	 * Gets the extents of this box.
	 *
	 * @return The box extents.
	 * @see GetCenter, GetCenterAndExtents, GetSize, GetVolume
	 */
	FORCEINLINE FVector GetExtent() const
	{
		return 0.5f * (Max - Min);
	}

	/**
	 * Gets a reference to the specified point of the bounding box.
	 *
	 * @param PointIndex The index of the extrema point to return.
	 * @return A reference to the point.
	 */
	FORCEINLINE FVector& GetExtrema(int PointIndex)
	{
		return (&Min)[PointIndex];
	}

	/**
	 * Gets a read-only reference to the specified point of the bounding box.
	 *
	 * @param PointIndex The index of extrema point to return.
	 * @return A read-only reference to the point.
	 */
	FORCEINLINE const FVector& GetExtrema(int PointIndex) const
	{
		return (&Min)[PointIndex];
	}

	/**
	 * Gets the size of this box.
	 *
	 * @return The box size.
	 * @see GetCenter, GetCenterAndExtents, GetExtent, GetVolume
	 */
	FORCEINLINE FVector GetSize() const
	{
		return (Max - Min);
	}

	/**
	 * Gets the volume of this box.
	 *
	 * @return The box volume.
	 * @see GetCenter, GetCenterAndExtents, GetExtent, GetSize
	 */
	FORCEINLINE float GetVolume() const
	{
		return ((Max.X - Min.X) * (Max.Y - Min.Y) * (Max.Z - Min.Z));
	}

	/**
	 * Set the initial values of the bounding box to Zero.
	 */
	FORCEINLINE void Init()
	{
		Min = Max = FVector::ZeroVector;
		IsValid = 0;
	}

	/**
	 * Checks whether the given location is inside this box.
	 *
	 * @param In The location to test for inside the bounding volume.
	 * @return true if location is inside this volume.
	 * @see IsInsideXY
	 */
	FORCEINLINE bool IsInside(const FVector& In) const
	{
		return ((In.X > Min.X) && (In.X < Max.X) && (In.Y > Min.Y) && (In.Y < Max.Y) && (In.Z > Min.Z) && (In.Z < Max.Z));
	}

	/**
	 * Checks whether the given location is inside or on this box.
	 *
	 * @param In The location to test for inside the bounding volume.
	 * @return true if location is inside this volume.
	 * @see IsInsideXY
	 */
	FORCEINLINE bool IsInsideOrOn(const FVector& In) const
	{
		return ((In.X >= Min.X) && (In.X <= Max.X) && (In.Y >= Min.Y) && (In.Y <= Max.Y) && (In.Z >= Min.Z) && (In.Z <= Max.Z));
	}

	/**
	 * Checks whether a given box is fully encapsulated by this box.
	 *
	 * @param Other The box to test for encapsulation within the bounding volume.
	 * @return true if box is inside this volume.
	 */
	FORCEINLINE bool IsInside(const FBox& Other) const
	{
		return (IsInside(Other.Min) && IsInside(Other.Max));
	}

	/**
	 * Checks whether the given location is inside this box in the XY plane.
	 *
	 * @param In The location to test for inside the bounding box.
	 * @return true if location is inside this box in the XY plane.
	 * @see IsInside
	 */
	FORCEINLINE bool IsInsideXY(const FVector& In) const
	{
		return ((In.X > Min.X) && (In.X < Max.X) && (In.Y > Min.Y) && (In.Y < Max.Y));
	}

	/**
	 * Checks whether the given box is fully encapsulated by this box in the XY plane.
	 *
	 * @param Other The box to test for encapsulation within the bounding box.
	 * @return true if box is inside this box in the XY plane.
	 */
	FORCEINLINE bool IsInsideXY(const FBox& Other) const
	{
		return (IsInsideXY(Other.Min) && IsInsideXY(Other.Max));
	}

public:

	/**
	 * Utility function to build an AABB from Origin and Extent
	 *
	 * @param Origin The location of the bounding box.
	 * @param Extent Half size of the bounding box.
	 * @return A new axis-aligned bounding box.
	 */
	static FBox BuildAABB(const FVector& Origin, const FVector& Extent)
	{
		FBox NewBox(Origin - Extent, Origin + Extent);

		return NewBox;
	}
};


/**
 * FBox specialization for TIsPODType trait.
 */
template<> struct TIsPODType<FBox> { enum { Value = true }; };