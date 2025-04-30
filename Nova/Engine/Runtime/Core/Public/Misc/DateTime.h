#pragma once

#include "Containers/UnrealString.h"
#include "CoreTypes.h"
#include "Templates/TypeHash.h"

class FArchive;
class FOutputDevice;
class UObject;


/**
 * Enumerates the days of the week in 7-day calendars.
 */
enum class EDayOfWeek
{
	Monday = 0,
	Tuesday,
	Wednesday,
	Thursday,
	Friday,
	Saturday,
	Sunday
};


/**
 * Enumerates the months of the year in 12-month calendars.
 */
enum class EMonthOfYear
{
	January = 1,
	February,
	March,
	April,
	May,
	June,
	July,
	August,
	September,
	October,
	November,
	December
};

struct FDateTime
{
public:

	/** Default constructor (zero initialization). */
	FDateTime()
		: Ticks(0)
	{
	}

	/**
	 * Creates and initializes a new instance with the specified number of ticks.
	 *
	 * @param InTicks The ticks representing the date and time.
	 */
	FDateTime(int64 InTicks)
		: Ticks(InTicks)
	{
	}

public:

	/**
	 * Compares this date with the given date for equality.
	 *
	 * @param Other The date to compare with.
	 * @return true if the dates are equal, false otherwise.
	 */
	bool operator==(const FDateTime& Other) const
	{
		return (Ticks == Other.Ticks);
	}

	/**
	 * Compares this date with the given date for inequality.
	 *
	 * @param Other The date to compare with.
	 * @return true if the dates are not equal, false otherwise.
	 */
	bool operator!=(const FDateTime& Other) const
	{
		return (Ticks != Other.Ticks);
	}

	/**
	 * Checks whether this date is greater than the given date.
	 *
	 * @param Other The date to compare with.
	 * @return true if this date is greater, false otherwise.
	 */
	bool operator>(const FDateTime& Other) const
	{
		return (Ticks > Other.Ticks);
	}

	/**
	 * Checks whether this date is greater than or equal to the date span.
	 *
	 * @param Other The date to compare with.
	 * @return true if this date is greater or equal, false otherwise.
	 */
	bool operator>=(const FDateTime& Other) const
	{
		return (Ticks >= Other.Ticks);
	}

	/**
	 * Checks whether this date is less than the given date.
	 *
	 * @param Other The date to compare with.
	 * @return true if this date is less, false otherwise.
	 */
	bool operator<(const FDateTime& Other) const
	{
		return (Ticks < Other.Ticks);
	}

	/**
	 * Checks whether this date is less than or equal to the given date.
	 *
	 * @param Other The date to compare with.
	 * @return true if this date is less or equal, false otherwise.
	 */
	bool operator<=(const FDateTime& Other) const
	{
		return (Ticks <= Other.Ticks);
	}

	bool IsValid() { return Ticks >= 630822816000000000LL; }

private:

	/** Holds the ticks in 100 nanoseconds resolution since January 1, 0001 A.D. */
	int64 Ticks;
};