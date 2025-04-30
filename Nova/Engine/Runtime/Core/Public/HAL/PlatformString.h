#pragma once
#include <cstdarg>
#include <tchar.h>

#include "UnrealMemory.h"
#include "GenericPlatform/GenericPlatformStricmp.h"

template <bool Dummy, typename T>
struct TIsFixedWidthEncoding_Helper
{
    enum { Value = false };
};

template <bool Dummy> struct TIsFixedWidthEncoding_Helper<Dummy, ANSICHAR> { enum { Value = true }; };
template <bool Dummy> struct TIsFixedWidthEncoding_Helper<Dummy, WIDECHAR> { enum { Value = true }; };
template <bool Dummy> struct TIsFixedWidthEncoding_Helper<Dummy, UCS2CHAR> { enum { Value = true }; };
#if PLATFORM_TCHAR_IS_CHAR16
template <bool Dummy> struct TIsFixedWidthEncoding_Helper<Dummy, wchar_t> { enum { Value = true }; };
#endif

template <typename T>
struct TIsFixedWidthEncoding : TIsFixedWidthEncoding_Helper<false, T>
{
};

struct FPlatformString
{

    static FORCEINLINE int32 Strcmp(const WIDECHAR* String1, const WIDECHAR* String2)
    {
        return wcscmp(String1, String2);
    }

    static FORCEINLINE int32 Strncmp(const WIDECHAR* String1, const WIDECHAR* String2, SIZE_T Count)
    {
        return wcsncmp(String1, String2, Count);
    }

    static FORCEINLINE int32 Strnicmp(const WIDECHAR* String1, const WIDECHAR* String2, SIZE_T Count)
    {
        return _wcsnicmp(String1, String2, Count);
    }

    template <typename CharType1, typename CharType2>
    static FORCEINLINE int32 Stricmp(const CharType1* String1, const CharType2* String2)
    {
        return FGenericPlatformStricmp::Stricmp(String1, String2);
    }

    template <typename CharType1>
    static FORCEINLINE int32 Stricmp(const std::wstring& String1, const CharType1* String2)
    {
        return FGenericPlatformStricmp::Stricmp(String1.c_str(), String2);
    }

    template <typename CharType1>
    static FORCEINLINE int32 Stricmp(const CharType1* String1, const std::wstring& String2)
    {
        return FGenericPlatformStricmp::Stricmp(String1, String2.c_str());
    }

    static FORCEINLINE int32 Strnicmp(const ANSICHAR* String1, const ANSICHAR* String2, SIZE_T Count)
    {
        return _strnicmp(String1, String2, Count);
    }

#define USE_SECURE_CRT 1

    /**
 * Ansi implementation
 **/
    static FORCEINLINE ANSICHAR* Strcpy(ANSICHAR* Dest, SIZE_T DestCount, const ANSICHAR* Src)
    {
#if USE_SECURE_CRT
        strcpy_s(Dest, DestCount, Src);
        return Dest;
#else
        return (ANSICHAR*)strcpy(Dest, Src);
#endif // USE_SECURE_CRT
    }

    static FORCEINLINE WIDECHAR* Strcpy(WIDECHAR* Dest, SIZE_T DestCount, const WIDECHAR* Src)
    {
#if USE_SECURE_CRT
        _tcscpy_s(Dest, DestCount, Src);
        return Dest;
#else
        return (WIDECHAR*)wcscpy(Dest, Src);
#endif // USE_SECURE_CRT
    }

    static FORCEINLINE WIDECHAR* Strncpy(WIDECHAR* Dest, const WIDECHAR* Src, SIZE_T MaxLen)
    {
#if USE_SECURE_CRT
        _tcsncpy_s(Dest, MaxLen, Src, MaxLen - 1);
#else
        wcsncpy(Dest, Src, MaxLen - 1);
        Dest[MaxLen - 1] = 0;
#endif // USE_SECURE_CRT
        return Dest;
    }

    static FORCEINLINE WIDECHAR* Strcat(WIDECHAR* Dest, SIZE_T DestCount, const WIDECHAR* Src)
    {
#if USE_SECURE_CRT
        _tcscat_s(Dest, DestCount, Src);
        return Dest;
#else
        return (WIDECHAR*)wcscat(Dest, Src);
#endif // USE_SECURE_CRT
    }

    static FORCEINLINE WIDECHAR* Strupr(WIDECHAR* Dest, SIZE_T DestCount)
    {
#if USE_SECURE_CRT
        _tcsupr_s(Dest, DestCount);
        return Dest;
#else
        return (WIDECHAR*)_wcsupr(Dest);
#endif // USE_SECURE_CRT
    }

    static FORCEINLINE void Strncpy(ANSICHAR* Dest, const ANSICHAR* Src, SIZE_T MaxLen)
    {
#if USE_SECURE_CRT
        strncpy_s(Dest, MaxLen, Src, MaxLen - 1);
#else
        strncpy(Dest, Src, MaxLen);
        Dest[MaxLen - 1] = 0;
#endif // USE_SECURE_CRT
    }

    static FORCEINLINE ANSICHAR* Strcat(ANSICHAR* Dest, SIZE_T DestCount, const ANSICHAR* Src)
    {
#if USE_SECURE_CRT
        strcat_s(Dest, DestCount, Src);
        return Dest;
#else
        return (ANSICHAR*)strcat(Dest, Src);
#endif // USE_SECURE_CRT
    }

    static FORCEINLINE ANSICHAR* Strupr(ANSICHAR* Dest, SIZE_T DestCount)
    {
#if USE_SECURE_CRT
        _strupr_s(Dest, DestCount);
        return Dest;
#else
        return (ANSICHAR*)strupr(Dest);
#endif // USE_SECURE_CRT
    }

    static FORCEINLINE const ANSICHAR* Strstr(const ANSICHAR* String, const ANSICHAR* Find)
    {
        return strstr(String, Find);
    }

    static FORCEINLINE const ANSICHAR* Strchr(const ANSICHAR* String, ANSICHAR C)
    {
        return strchr(String, C);
    }

    static FORCEINLINE const ANSICHAR* Strrchr(const ANSICHAR* String, ANSICHAR C)
    {
        return strrchr(String, C);
    }

    static FORCEINLINE const WIDECHAR* Strstr(const WIDECHAR* String, const WIDECHAR* Find)
    {
        return wcsstr(String, Find);
    }

    static FORCEINLINE const WIDECHAR* Strchr(const WIDECHAR* String, WIDECHAR C)
    {
        return wcschr(String, C);
    }

    static FORCEINLINE const WIDECHAR* Strrchr(const WIDECHAR* String, WIDECHAR C)
    {
        return wcsrchr(String, C);
    }

    static FORCEINLINE int32 Strlen(const ANSICHAR* String)
    {
        return strlen(String);
    }

    static FORCEINLINE int32 Strlen(const WIDECHAR* String)
    {
        return wcslen(String);
    }

    static FORCEINLINE int32 Strlen(const UCS2CHAR* String)
    {
        return wcslen((const WIDECHAR*)String);
    }

    static FORCEINLINE int32 Atoi(const ANSICHAR* String)
    {
        return atoi(String);
    }

    static FORCEINLINE int64 Atoi64(const ANSICHAR* String)
    {
        return _strtoi64(String, NULL, 10);
    }

    static FORCEINLINE float Atof(const ANSICHAR* String)
    {
        return (float)atof(String);
    }

    static FORCEINLINE double Atod(const ANSICHAR* String)
    {
        return atof(String);
    }

    static FORCEINLINE int32 Strtoi(const ANSICHAR* Start, ANSICHAR** End, int32 Base)
    {
        return strtol(Start, End, Base);
    }

    static FORCEINLINE int64 Strtoi64(const ANSICHAR* Start, ANSICHAR** End, int32 Base)
    {
        return _strtoi64(Start, End, Base);
    }

    static FORCEINLINE uint64 Strtoui64(const ANSICHAR* Start, ANSICHAR** End, int32 Base)
    {
        return _strtoui64(Start, End, Base);
    }

    static FORCEINLINE ANSICHAR* Strtok(ANSICHAR* StrToken, const ANSICHAR* Delim, ANSICHAR** Context)
    {
        return strtok_s(StrToken, Delim, Context);
    }

    static FORCEINLINE int32 GetVarArgs(ANSICHAR* Dest, SIZE_T DestSize, int32 Count, const ANSICHAR*& Fmt, va_list ArgPtr)
    {
#if USE_SECURE_CRT
        int32 Result = _vsnprintf_s(Dest, DestSize, Count, Fmt, ArgPtr);
#else
        int32 Result = _vsnprintf(Dest, Count, Fmt, ArgPtr);
#endif // USE_SECURE_CRT
        va_end(ArgPtr);
        return Result;
    }

    static FORCEINLINE int32 Atoi(const WIDECHAR* String)
    {
        return _wtoi(String);
    }

    static FORCEINLINE int64 Atoi64(const WIDECHAR* String)
    {
        return _wtoi64(String);
    }

    static FORCEINLINE float Atof(const WIDECHAR* String)
    {
        return _wtof(String);
    }

    static FORCEINLINE double Atod(const WIDECHAR* String)
    {
        return wcstod(String, NULL);
    }

    static FORCEINLINE int32 Strtoi(const WIDECHAR* Start, WIDECHAR** End, int32 Base)
    {
        return wcstoul(Start, End, Base);
    }

    static FORCEINLINE int64 Strtoi64(const WIDECHAR* Start, WIDECHAR** End, int32 Base)
    {
        return _wcstoi64(Start, End, Base);
    }

    static FORCEINLINE uint64 Strtoui64(const WIDECHAR* Start, WIDECHAR** End, int32 Base)
    {
        return _wcstoui64(Start, End, Base);
    }

    static FORCEINLINE WIDECHAR* Strtok(WIDECHAR* StrToken, const WIDECHAR* Delim, WIDECHAR** Context)
    {
        return wcstok_s(StrToken, Delim, Context);
    }

    static FORCEINLINE int32 GetVarArgs(WIDECHAR* Dest, SIZE_T DestSize, int32 Count, const WIDECHAR*& Fmt, va_list ArgPtr)
    {
#if USE_SECURE_CRT
        int32 Result = _vsntprintf_s(Dest, DestSize, Count, Fmt, ArgPtr);
#else
        int32 Result = _vsnwprintf(Dest, Count, Fmt, ArgPtr);
#endif // USE_SECURE_CRT
        va_end(ArgPtr);
        return Result;
    }

    template <typename DestEncoding, typename SourceEncoding>
    static typename TEnableIf<TIsFixedWidthEncoding<SourceEncoding>::Value&& TIsFixedWidthEncoding<DestEncoding>::Value, int32>::Type ConvertedLength(const SourceEncoding* Src, int32 SrcSize)
    {
        return SrcSize;
    }

    static FORCEINLINE void* Memcpy(void* Dest, const void* Src, SIZE_T Count)
    {
        return FMemory::Memcpy(Dest, Src, Count);
    }

    template <typename EncodingA, typename EncodingB>
    struct TAreEncodingsCompatible
    {
        enum { Value = TIsFixedWidthEncoding<EncodingA>::Value && TIsFixedWidthEncoding<EncodingB>::Value && sizeof(EncodingA) == sizeof(EncodingB) };
    };

    template <typename SourceEncoding, typename DestEncoding>
    static FORCEINLINE typename TEnableIf<
        // This overload should be called when SourceEncoding and DestEncoding are 'compatible', i.e. they're the same type or equivalent (e.g. like UCS2CHAR and WIDECHAR are on Windows).
        TAreEncodingsCompatible<SourceEncoding, DestEncoding>::Value,
        DestEncoding*
    >::Type Convert(DestEncoding* Dest, int32 DestSize, const SourceEncoding* Src, int32 SrcSize, DestEncoding BogusChar = (DestEncoding)'?')
    {
        if (DestSize < SrcSize)
            return nullptr;

        return (DestEncoding*)Memcpy(Dest, Src, SrcSize * sizeof(SourceEncoding)) + SrcSize;
    }
};