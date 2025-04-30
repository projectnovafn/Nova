#pragma once
#include <corecrt_math.h>
#include <cstdlib>

#include "Platform.h"
#include "intrin.h"

/*-----------------------------------------------------------------------------
    Floating point constants.
-----------------------------------------------------------------------------*/

#undef  PI
#define PI 					(3.1415926535897932f)
#define SMALL_NUMBER		(1.e-8f)
#define KINDA_SMALL_NUMBER	(1.e-4f)
#define BIG_NUMBER			(3.4e+38f)
#define EULERS_NUMBER       (2.71828182845904523536f)

// Copied from float.h
#define MAX_FLT 3.402823466e+38F

// Aux constants.
#define INV_PI			(0.31830988618f)
#define HALF_PI			(1.57079632679f)

// Magic numbers for numerical precision.
#define DELTA			(0.00001f)

/**
 * Lengths of normalized vectors (These are half their maximum values
 * to assure that dot products with normalized vectors don't overflow).
 */
#define FLOAT_NORMAL_THRESH				(0.0001f)

 //
 // Magic numbers for numerical precision.
 //
#define THRESH_POINT_ON_PLANE			(0.10f)		/* Thickness of plane for front/back/inside test */
#define THRESH_POINT_ON_SIDE			(0.20f)		/* Thickness of polygon side's side-plane for point-inside/outside/on side test */
#define THRESH_POINTS_ARE_SAME			(0.00002f)	/* Two points are same if within this distance */
#define THRESH_POINTS_ARE_NEAR			(0.015f)	/* Two points are near if within this distance and can be combined if imprecise math is ok */
#define THRESH_NORMALS_ARE_SAME			(0.00002f)	/* Two normal points are same if within this distance */
#define THRESH_UVS_ARE_SAME			    (0.0009765625f)/* Two UV are same if within this threshold (1.0f/1024f) */
/* Making this too large results in incorrect CSG classification and disaster */
#define THRESH_VECTORS_ARE_NEAR			(0.0004f)	/* Two vectors are near if within this distance and can be combined if imprecise math is ok */
/* Making this too large results in lighting problems due to inaccurate texture coordinates */
#define THRESH_SPLIT_POLY_WITH_PLANE	(0.25f)		/* A plane splits a polygon in half */
#define THRESH_SPLIT_POLY_PRECISELY		(0.01f)		/* A plane exactly splits a polygon */
#define THRESH_ZERO_NORM_SQUARED		(0.0001f)	/* Size of a unit normal that is considered "zero", squared */
#define THRESH_NORMALS_ARE_PARALLEL		(0.999845f)	/* Two unit vectors are parallel if abs(A dot B) is greater than or equal to this. This is roughly cosine(1.0 degrees). */
#define THRESH_NORMALS_ARE_ORTHOGONAL	(0.017455f)	/* Two unit vectors are orthogonal (perpendicular) if abs(A dot B) is less than or equal this. This is roughly cosine(89.0 degrees). */

#define THRESH_VECTOR_NORMALIZED		(0.01f)		/** Allowed error for a normalized vector (against squared magnitude) */
#define THRESH_QUAT_NORMALIZED			(0.01f)		/** Allowed error for a normalized quaternion (against squared magnitude) */

static int32 GSRandSeed;

struct FPlatformMath
{
    static CONSTEXPR FORCEINLINE int32 TruncToInt(float F)
    {
        return (int32)F;
    }

    static CONSTEXPR FORCEINLINE float TruncToFloat(float F)
    {
        return (float)TruncToInt(F);
    }

    static FORCEINLINE uint32 CountLeadingZeros(uint32 Value)
    {
        // Use BSR to return the log2 of the integer
        unsigned long Log2;
        if (_BitScanReverse(&Log2, Value) != 0)
        {
            return 31 - Log2;
        }

        return 32;
    }
    static FORCEINLINE uint32 CountTrailingZeros(uint32 Value)
    {
        if (Value == 0)
        {
            return 32;
        }
        unsigned long BitIndex;	// 0-based, where the LSB is 0 and MSB is 31
        _BitScanForward(&BitIndex, Value);	// Scans from LSB to MSB
        return BitIndex;
    }
    static FORCEINLINE uint32 CeilLogTwo(uint32 Arg)
    {
        int32 Bitmask = ((int32)(CountLeadingZeros(Arg) << 26)) >> 31;
        return (32 - CountLeadingZeros(Arg - 1)) & (~Bitmask);
    }
    static FORCEINLINE uint32 RoundUpToPowerOfTwo(uint32 Arg)
    {
        return 1 << CeilLogTwo(Arg);
    }

    template< class T >
    static CONSTEXPR FORCEINLINE T Abs(const T A)
    {
        return (A >= (T)0) ? A : -A;
    }

    template< class T >
    static CONSTEXPR FORCEINLINE T Sign(const T A)
    {
        return (A > (T)0) ? (T)1 : ((A < (T)0) ? (T)-1 : (T)0);
    }

    template< class T >
    static CONSTEXPR FORCEINLINE T Max(const T A, const T B)
    {
        return (A >= B) ? A : B;
    }

    template< class T >
    static CONSTEXPR FORCEINLINE T Min(const T A, const T B)
    {
        return (A <= B) ? A : B;
    }

    template< class T >
    static FORCEINLINE T Square(const T A)
    {
        return A * A;
    }

    template< class T >
    static FORCEINLINE T Clamp(const T X, const T Min, const T Max)
    {
        return X < Min ? Min : X < Max ? X : Max;
    }

    template< class T, class U >
    static FORCEINLINE T Lerp(const T& A, const T& B, const U& Alpha)
    {
        return (T)(A + Alpha * (B - A));
    }

    /** Divides two integers and rounds up */
    template <class T>
    static FORCEINLINE T DivideAndRoundUp(T Dividend, T Divisor)
    {
        return (Dividend + Divisor - 1) / Divisor;
    }

    /** Divides two integers and rounds down */
    template <class T>
    static FORCEINLINE T DivideAndRoundDown(T Dividend, T Divisor)
    {
        return Dividend / Divisor;
    }

    /** Divides two integers and rounds to nearest */
    template <class T>
    static FORCEINLINE T DivideAndRoundNearest(T Dividend, T Divisor)
    {
        return (Dividend >= 0)
            ? (Dividend + Divisor / 2) / Divisor
            : (Dividend - Divisor / 2 + 1) / Divisor;
    }


    template <typename T>
    static FORCEINLINE bool IsPowerOfTwo(T Value)
    {
        return ((Value & (Value - 1)) == (T)0);
    }


    // Math Operations

    /** Returns highest of 3 values */
    template< class T >
    static FORCEINLINE T Max3(const T A, const T B, const T C)
    {
        return Max(Max(A, B), C);
    }

    /** Returns lowest of 3 values */
    template< class T >
    static FORCEINLINE T Min3(const T A, const T B, const T C)
    {
        return Min(Min(A, B), C);
    }

    static FORCEINLINE float Fractional(float Value)
    {
        return Value - TruncToFloat(Value);
    }

    // Returns e^Value
    static FORCEINLINE float Exp(float Value) { return expf(Value); }
    // Returns 2^Value
    static FORCEINLINE float Exp2(float Value) { return powf(2.f, Value); /*exp2f(Value);*/ }
    static FORCEINLINE float Loge(float Value) { return logf(Value); }
    static FORCEINLINE float LogX(float Base, float Value) { return Loge(Value) / Loge(Base); }
    // 1.0 / Loge(2) = 1.4426950f
    static FORCEINLINE float Log2(float Value) { return Loge(Value) * 1.4426950f; }

    static FORCEINLINE float Sin(float Value) { return sinf(Value); }
    static FORCEINLINE float Asin(float Value) { return asinf((Value < -1.f) ? -1.f : ((Value < 1.f) ? Value : 1.f)); }
    static FORCEINLINE float Sinh(float Value) { return sinhf(Value); }
    static FORCEINLINE float Cos(float Value) { return cosf(Value); }
    static FORCEINLINE float Acos(float Value) { return acosf((Value < -1.f) ? -1.f : ((Value < 1.f) ? Value : 1.f)); }
    static FORCEINLINE float Tan(float Value) { return tanf(Value); }
    static FORCEINLINE float Atan(float Value) { return atanf(Value); }
    static float Atan2(float Y, float X)
    {
        //return atan2f(Y,X);
        // atan2f occasionally returns NaN with perfectly valid input (possibly due to a compiler or library bug).
        // We are replacing it with a minimax approximation with a max relative error of 7.15255737e-007 compared to the C library function.
        // On PC this has been measured to be 2x faster than the std C version.

        const float absX = FPlatformMath::Abs(X);
        const float absY = FPlatformMath::Abs(Y);
        const bool yAbsBigger = (absY > absX);
        float t0 = yAbsBigger ? absY : absX; // Max(absY, absX)
        float t1 = yAbsBigger ? absX : absY; // Min(absX, absY)

        if (t0 == 0.f)
            return 0.f;

        float t3 = t1 / t0;
        float t4 = t3 * t3;

        static const float c[7] = {
                +7.2128853633444123e-03f,
                -3.5059680836411644e-02f,
                +8.1675882859940430e-02f,
                -1.3374657325451267e-01f,
                +1.9856563505717162e-01f,
                -3.3324998579202170e-01f,
                +1.0f
        };

        t0 = c[0];
        t0 = t0 * t4 + c[1];
        t0 = t0 * t4 + c[2];
        t0 = t0 * t4 + c[3];
        t0 = t0 * t4 + c[4];
        t0 = t0 * t4 + c[5];
        t0 = t0 * t4 + c[6];
        t3 = t0 * t3;

        t3 = yAbsBigger ? (0.5f * PI) - t3 : t3;
        t3 = (X < 0.0f) ? PI - t3 : t3;
        t3 = (Y < 0.0f) ? -t3 : t3;

        return t3;
    }

    // Note:  We use FASTASIN_HALF_PI instead of HALF_PI inside of FastASin(), since it was the value that accompanied the minimax coefficients below.
    // It is important to use exactly the same value in all places inside this function to ensure that FastASin(0.0f) == 0.0f.
    // For comparison:
    //		HALF_PI				== 1.57079632679f == 0x3fC90FDB
    //		FASTASIN_HALF_PI	== 1.5707963050f  == 0x3fC90FDA
#define FASTASIN_HALF_PI (1.5707963050f)
    /**
    * Computes the ASin of a scalar float.
    *
    * @param Value  input angle
    * @return ASin of Value
    */
    static FORCEINLINE float FastAsin(float Value)
    {
        // Clamp input to [-1,1].
        bool nonnegative = (Value >= 0.0f);
        float x = FPlatformMath::Abs(Value);
        float omx = 1.0f - x;
        if (omx < 0.0f)
        {
            omx = 0.0f;
        }
        float root = FPlatformMath::Sqrt(omx);
        // 7-degree minimax approximation
        float result = ((((((-0.0012624911f * x + 0.0066700901f) * x - 0.0170881256f) * x + 0.0308918810f) * x - 0.0501743046f) * x + 0.0889789874f) * x - 0.2145988016f) * x + FASTASIN_HALF_PI;
        result *= root;  // acos(|x|)
        // acos(x) = pi - acos(-x) when x < 0, asin(x) = pi/2 - acos(x)
        return (nonnegative ? FASTASIN_HALF_PI - result : result - FASTASIN_HALF_PI);
    }
#undef FASTASIN_HALF_PI

    static FORCEINLINE float Sqrt(float Value) { return sqrtf(Value); }
    static FORCEINLINE float Pow(float A, float B) { return powf(A, B); }

    /** Computes a fully accurate inverse square root */
    static FORCEINLINE float InvSqrt(float F)
    {
        return 1.0f / sqrtf(F);
    }

    /** Computes a faster but less accurate inverse square root */
    static FORCEINLINE float InvSqrtEst(float F)
    {
        return InvSqrt(F);
    }

    /** Return true if value is NaN (not a number). */
    static FORCEINLINE bool IsNaN(float A)
    {
        return ((*(uint32*)&A) & 0x7FFFFFFF) > 0x7F800000;
    }
    /** Return true if value is finite (not NaN and not Infinity). */
    static FORCEINLINE bool IsFinite(float A)
    {
        return ((*(uint32*)&A) & 0x7F800000) != 0x7F800000;
    }
    static FORCEINLINE bool IsNegativeFloat(const float& A)
    {
        return ((*(uint32*)&A) >= (uint32)0x80000000); // Detects sign bit.
    }

    static FORCEINLINE bool IsNegativeDouble(const double& A)
    {
        return ((*(uint64*)&A) >= (uint64)0x8000000000000000); // Detects sign bit.
    }

    static FORCEINLINE bool IsNearlyZero(const float& A, const float& Tolerance = 1e-6f)
    {
        return Abs(A) < Tolerance;
    }

    /**
     * Computes the base 2 logarithm for an integer value that is greater than 0.
     * The result is rounded down to the nearest integer.
     *
     * @param Value		The value to compute the log of
     * @return			Log2 of Value. 0 if Value is 0.
     */
    static FORCEINLINE uint32 FloorLog2(uint32 Value)
    {
        /*		// reference implementation
                // 1500ms on test data
                uint32 Bit = 32;
                for (; Bit > 0;)
                {
                    Bit--;
                    if (Value & (1<<Bit))
                    {
                        break;
                    }
                }
                return Bit;
        */
        // same output as reference

        // see http://codinggorilla.domemtech.com/?p=81 or http://en.wikipedia.org/wiki/Binary_logarithm but modified to return 0 for a input value of 0
        // 686ms on test data
        uint32 pos = 0;
        if (Value >= 1 << 16) { Value >>= 16; pos += 16; }
        if (Value >= 1 << 8) { Value >>= 8; pos += 8; }
        if (Value >= 1 << 4) { Value >>= 4; pos += 4; }
        if (Value >= 1 << 2) { Value >>= 2; pos += 2; }
        if (Value >= 1 << 1) { pos += 1; }
        return (Value == 0) ? 0 : pos;

        // even faster would be method3 but it can introduce more cache misses and it would need to store the table somewhere
        // 304ms in test data
        /*int LogTable256[256];

        void prep()
        {
            LogTable256[0] = LogTable256[1] = 0;
            for (int i = 2; i < 256; i++)
            {
                LogTable256[i] = 1 + LogTable256[i / 2];
            }
            LogTable256[0] = -1; // if you want log(0) to return -1
        }

        int _forceinline method3(uint32 v)
        {
            int r;     // r will be lg(v)
            uint32 tt; // temporaries

            if ((tt = v >> 24) != 0)
            {
                r = (24 + LogTable256[tt]);
            }
            else if ((tt = v >> 16) != 0)
            {
                r = (16 + LogTable256[tt]);
            }
            else if ((tt = v >> 8 ) != 0)
            {
                r = (8 + LogTable256[tt]);
            }
            else
            {
                r = LogTable256[v];
            }
            return r;
        }*/
    }

    /**
     * Computes the base 2 logarithm for a 64-bit value that is greater than 0.
     * The result is rounded down to the nearest integer.
     *
     * @param Value		The value to compute the log of
     * @return			Log2 of Value. 0 if Value is 0.
     */
    static FORCEINLINE uint64 FloorLog2_64(uint64 Value)
    {
        uint64 pos = 0;
        if (Value >= 1ull << 32) { Value >>= 32; pos += 32; }
        if (Value >= 1ull << 16) { Value >>= 16; pos += 16; }
        if (Value >= 1ull << 8) { Value >>= 8; pos += 8; }
        if (Value >= 1ull << 4) { Value >>= 4; pos += 4; }
        if (Value >= 1ull << 2) { Value >>= 2; pos += 2; }
        if (Value >= 1ull << 1) { pos += 1; }
        return (Value == 0) ? 0 : pos;
    }

    static FORCEINLINE void SinCos(float* ScalarSin, float* ScalarCos, float  Value)
    {
        // Map Value to y in [-pi,pi], x = 2*pi*quotient + remainder.
        float quotient = (INV_PI * 0.5f) * Value;
        if (Value >= 0.0f)
        {
            quotient = (float)((int)(quotient + 0.5f));
        }
        else
        {
            quotient = (float)((int)(quotient - 0.5f));
        }
        float y = Value - (2.0f * PI) * quotient;

        // Map y to [-pi/2,pi/2] with sin(y) = sin(Value).
        float sign;
        if (y > HALF_PI)
        {
            y = PI - y;
            sign = -1.0f;
        }
        else if (y < -HALF_PI)
        {
            y = -PI - y;
            sign = -1.0f;
        }
        else
        {
            sign = +1.0f;
        }

        float y2 = y * y;

        // 11-degree minimax approximation
        *ScalarSin = (((((-2.3889859e-08f * y2 + 2.7525562e-06f) * y2 - 0.00019840874f) * y2 + 0.0083333310f) * y2 - 0.16666667f) * y2 + 1.0f) * y;

        // 10-degree minimax approximation
        float p = ((((-2.6051615e-07f * y2 + 2.4760495e-05f) * y2 - 0.0013888378f) * y2 + 0.041666638f) * y2 - 0.5f) * y2 + 1.0f;
        *ScalarCos = sign * p;
    }

    // Conversion Functions

    /**
     * Converts radians to degrees.
     * @param	RadVal			Value in radians.
     * @return					Value in degrees.
     */
    template<class T>
    static FORCEINLINE auto RadiansToDegrees(T const& RadVal) -> decltype(RadVal* (180.f / PI))
    {
        return RadVal * (180.f / PI);
    }

    /**
     * Converts degrees to radians.
     * @param	DegVal			Value in degrees.
     * @return					Value in radians.
     */
    template<class T>
    static FORCEINLINE auto DegreesToRadians(T const& DegVal) -> decltype(DegVal* (PI / 180.f))
    {
        return DegVal * (PI / 180.f);
    }

    static FORCEINLINE int32 CountBits(uint64 Bits)
    {
        // https://en.wikipedia.org/wiki/Hamming_weight
        Bits -= (Bits >> 1) & 0x5555555555555555ull;
        Bits = (Bits & 0x3333333333333333ull) + ((Bits >> 2) & 0x3333333333333333ull);
        Bits = (Bits + (Bits >> 4)) & 0x0f0f0f0f0f0f0f0full;
        return (Bits * 0x0101010101010101) >> 56;
    }

    static CONSTEXPR FORCEINLINE float FloatSelect(float Comparand, float ValueGEZero, float ValueLTZero)
    {
        return Comparand >= 0.f ? ValueGEZero : ValueLTZero;
    }

    static FORCEINLINE int32 RoundToInt(float F)
    {
        // Note: the x2 is to workaround the rounding-to-nearest-even-number issue when the fraction is .5
        return _mm_cvt_ss2si(_mm_set_ss(F + F + 0.5f)) >> 1;
    }

    static FORCEINLINE float RoundToFloat(float F)
    {
        return (float)RoundToInt(F);
    }

    static FORCEINLINE int32 FloorToInt(float F)
    {
        return _mm_cvt_ss2si(_mm_set_ss(F + F - 0.5f)) >> 1;
    }

    static FORCEINLINE float FloorToFloat(float F)
    {
        return (float)FloorToInt(F);
    }

    static FORCEINLINE float GridSnap(float Location, float Grid)
    {
        if (Grid == 0.f)	return Location;
        else
        {
            return FloorToFloat((Location + 0.5f * Grid) / Grid) * Grid;
        }
    }

    static void FmodReportError(float X, float Y)
    {
        if (Y == 0)
        {
            //ensureMsgf(Y != 0, TEXT("FMath::FMod(X=%f, Y=%f) : Y is zero, this is invalid and would result in NaN!"), X, Y);
        }
    }

    static FORCEINLINE float Fmod(float X, float Y)
    {
        if (fabsf(Y) <= 1.e-8f)
        {
            FmodReportError(X, Y);
            return 0.f;
        }
        const float Quotient = TruncToFloat(X / Y);
        float IntPortion = Y * Quotient;

        // Rounding and imprecision could cause IntPortion to exceed X and cause the result to be outside the expected range.
        // For example Fmod(55.8, 9.3) would result in a very small negative value!
        if (fabsf(IntPortion) > fabsf(X))
        {
            IntPortion = X;
        }

        const float Result = X - IntPortion;
        return Result;
    }

    /** Returns a random integer between 0 and RAND_MAX, inclusive */
    static FORCEINLINE int32 Rand() { return rand(); }

    /** Seeds global random number functions Rand() and FRand() */
    static FORCEINLINE void RandInit(int32 Seed) { srand(Seed); }

    /** Returns a random float between 0 and 1, inclusive. */
    static FORCEINLINE float FRand() { return Rand() / (float)RAND_MAX; }

    static void SRandInit(int32 Seed)
    {
        GSRandSeed = Seed;
    }

    static int32 GetRandSeed()
    {
        return GSRandSeed;
    }

    static float SRand()
    {
        GSRandSeed = (GSRandSeed * 196314165) + 907633515;
        union { float f; int32 i; } Result;
        union { float f; int32 i; } Temp;
        const float SRandTemp = 1.0f;
        Temp.f = SRandTemp;
        Result.i = (Temp.i & 0xff800000) | (GSRandSeed & 0x007fffff);
        return FPlatformMath::Fractional(Result.f);
    }
};

typedef FPlatformMath FMath;