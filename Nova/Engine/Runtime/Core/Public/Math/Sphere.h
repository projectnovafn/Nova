// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreTypes.h"
#include "Math/Vector.h"

/**
 * Implements a basic sphere.
 */
class FSphere
{
public:

	/** The sphere's center point. */
	FVector Center;

	/** The sphere's radius. */
	float W;

public:

	/** Default constructor (no initialization). */
	FSphere() {}

	/**
	 * Creates and initializes a new sphere.
	 *
	 * @param int32 Passing int32 sets up zeroed sphere.
	 */
	FSphere(int32)
		: Center(0.0f, 0.0f, 0.0f)
		, W(0)
	{
	}

	/**
	 * Creates and initializes a new sphere with the specified parameters.
	 *
	 * @param InV Center of sphere.
	 * @param InW Radius of sphere.
	 */
	FSphere(FVector InV, float InW)
		: Center(InV)
		, W(InW)
	{
	}

	/**
	 * Constructor.
	 *
	 * @param EForceInit Force Init Enum.
	 */
	explicit FORCEINLINE FSphere(EForceInit)
		: Center(ForceInit)
		, W(0.0f)
	{
	}


public:

	/**
	 * Check whether two spheres are the same within specified tolerance.
	 *
	 * @param Sphere The other sphere.
	 * @param Tolerance Error Tolerance.
	 * @return true if spheres are equal within specified tolerance, otherwise false.
	 */
	bool Equals(const FSphere& Sphere, float Tolerance = KINDA_SMALL_NUMBER) const
	{
		return Center.Equals(Sphere.Center, Tolerance) && FMath::Abs(W - Sphere.W) <= Tolerance;
	}

	/**
	 * Check whether sphere is inside of another.
	 *
	 * @param Other The other sphere.
	 * @param Tolerance Error Tolerance.
	 * @return true if sphere is inside another, otherwise false.
	 */
	bool IsInside(const FSphere& Other, float Tolerance = KINDA_SMALL_NUMBER) const
	{
		if (W > Other.W + Tolerance)
		{
			return false;
		}

		return (Center - Other.Center).SizeSquared() <= FMath::Square(Other.W + Tolerance - W);
	}

	/**
	* Checks whether the given location is inside this sphere.
	*
	* @param In The location to test for inside the bounding volume.
	* @return true if location is inside this volume.
	*/
	bool IsInside(const FVector& In, float Tolerance = KINDA_SMALL_NUMBER) const
	{
		return (Center - In).SizeSquared() <= FMath::Square(W + Tolerance);
	}

	/**
	 * Test whether this sphere intersects another.
	 *
	 * @param  Other The other sphere.
	 * @param  Tolerance Error tolerance.
	 * @return true if spheres intersect, false otherwise.
	 */
	FORCEINLINE bool Intersects(const FSphere& Other, float Tolerance = KINDA_SMALL_NUMBER) const
	{
		return (Center - Other.Center).SizeSquared() <= FMath::Square(FMath::Max(0.f, Other.W + W + Tolerance));
	}

public:
};