#pragma once

/*
   Memcury is a single-header file library for memory manipulation in C++.

   Containers:
       -PE::Address: A pointer container.
       -PE::Section: Portable executable section container for internal usage.

   Modules:
       -Scanner:
           -Constructors:
               -Default: Takes a pointer to start the scanning from.
               -FindPattern: Finds a pattern in memory.
               -FindStringRef: Finds a string reference in memory, supports all types of strings.
           -Functions:
               -SetTargetModule: Sets the target module for the scanner.
               -ScanFor: Scans for a byte(s) near the current address.
               -FindFunctionBoundary: Finds the boundary of a function near the current address.
               -RelativeOffset: Gets the relative offset of the current address.
               -AbsoluteOffset: Gets the absolute offset of the current address.
               -GetAs: Gets the current address as a type.
               -Get: Gets the current address as an int64.

       -TrampolineHook:
           -Constructors:
               -Default: Takes a pointer pointer to the target function and a pointer to the hook function.
           -Functions:
               -Commit: Commits the hook.
               -Revert: Reverts the hook.
               -Toggle: Toggles the hook on\off.

       -VEHHook:
           -Functions:
               -Init: Initializes the VEH Hook system.
               -AddHook: Adds a hook to the VEH Hook system.
               -RemoveHook: Removes a hook from the VEH Hook system.
*/

#include <string>
#include <format>
#include <vector>
#include <stdexcept>
#include <type_traits>
#include <intrin.h>
#include <Windows.h>
#include <source_location>
#include <DbgHelp.h>
#pragma comment(lib, "Dbghelp.lib")

#define VALID_POINTER(ptr) !IsBadReadPtr(ptr, 8)

#define MemcuryAssert(cond)                                              \
    if (!(cond))                                                         \
    {                                                                    \
        MessageBoxA(nullptr, #cond, __FUNCTION__, MB_ICONERROR | MB_OK); \
        Memcury::Safety::FreezeCurrentThread();                          \
    }

#define MemcuryAssertM(cond, msg)                                      \
    if (!(cond))                                                       \
    {                                                                  \
        MessageBoxA(nullptr, msg, __FUNCTION__, MB_ICONERROR | MB_OK); \
        Memcury::Safety::FreezeCurrentThread();                        \
    }

#define MemcuryThrow(msg)                                          \
    MessageBoxA(nullptr, msg, __FUNCTION__, MB_ICONERROR | MB_OK); \
    Memcury::Safety::FreezeCurrentThread();

namespace Memcury
{
    extern "C" IMAGE_DOS_HEADER __ImageBase;

    inline auto GetCurrentModule() -> HMODULE
    {
        return reinterpret_cast<HMODULE>(&__ImageBase);
    }

    namespace Util
    {
        static int GetOperandSize(char asmByte, const char* instructionBytes, size_t instructionSize, size_t& bytesRead)
        {
            unsigned char modRM = static_cast<unsigned char>(asmByte);
            int mod = (modRM >> 6) & 0x03;
            int rm = modRM & 0x07;

            if (mod == 0x03)
            {
                if (instructionSize >= 2)
                {
                    bytesRead = 2;
                    return (rm >= 4) ? 8 : 1 << (rm >> 1);
                }
            }
            else
            {
                if (instructionSize >= 2)
                {
                    unsigned char sibByte = static_cast<unsigned char>(instructionBytes[1]);
                    int scale = (sibByte >> 6) & 0x03;
                    int index = (sibByte >> 3) & 0x07;
                    int base = sibByte & 0x07;

                    if (index == 0x04)
                        index = -1;

                    if (base == 0x05)
                        base = -1;

                    bytesRead = 2;
                    return -1;
                }
            }

            return -1;
        }

        template <typename T>
        constexpr static auto IsInRange(T value, T min, T max) -> bool
        {
            return value >= min && value < max;
        }

        constexpr auto StrHash(const char* str, int h = 0) -> unsigned int
        {
            return !str[h] ? 5381 : (StrHash(str, h + 1) * 33) ^ str[h];
        }

        inline auto IsSamePage(void* A, void* B) -> bool
        {
            MEMORY_BASIC_INFORMATION InfoA;
            if (!VirtualQuery(A, &InfoA, sizeof(InfoA)))
            {
                return true;
            }

            MEMORY_BASIC_INFORMATION InfoB;
            if (!VirtualQuery(B, &InfoB, sizeof(InfoB)))
            {
                return true;
            }

            return InfoA.BaseAddress == InfoB.BaseAddress;
        }

        inline auto GetModuleStartAndEnd() -> std::pair<uintptr_t, uintptr_t>
        {
            auto HModule = GetCurrentModule();
            auto NTHeaders = reinterpret_cast<PIMAGE_NT_HEADERS>((uintptr_t)HModule + reinterpret_cast<PIMAGE_DOS_HEADER>((uintptr_t)HModule)->e_lfanew);

            uintptr_t dllStart = (uintptr_t)HModule;
            uintptr_t dllEnd = (uintptr_t)HModule + NTHeaders->OptionalHeader.SizeOfImage;

            return { dllStart, dllEnd };
        }

        inline auto CopyToClipboard(std::string str)
        {
            auto mem = GlobalAlloc(GMEM_FIXED, str.size() + 1);
            memcpy(mem, str.c_str(), str.size() + 1);

            OpenClipboard(nullptr);
            EmptyClipboard();
            SetClipboardData(CF_TEXT, mem);
            CloseClipboard();

            GlobalFree(mem);
        }
    }

    namespace Safety
    {
        enum class ExceptionMode
        {
            None,
            CatchDllExceptionsOnly,
            CatchAllExceptions
        };

        static auto FreezeCurrentThread() -> void
        {
            SuspendThread(GetCurrentThread());
        }

        static auto PrintStack(CONTEXT* ctx) -> void
        {
            STACKFRAME64 stack;
            memset(&stack, 0, sizeof(STACKFRAME64));

            auto process = GetCurrentProcess();
            auto thread = GetCurrentThread();

            SymInitialize(process, NULL, TRUE);

            bool result;
            DWORD64 displacement = 0;

            char buffer[sizeof(SYMBOL_INFO) + MAX_SYM_NAME * sizeof(TCHAR)]{ 0 };
            char name[256]{ 0 };
            char module[256]{ 0 };

            PSYMBOL_INFO symbolInfo = (PSYMBOL_INFO)buffer;

            for (ULONG frame = 0;; frame++)
            {
                result = StackWalk64(
                    IMAGE_FILE_MACHINE_AMD64,
                    process,
                    thread,
                    &stack,
                    ctx,
                    NULL,
                    SymFunctionTableAccess64,
                    SymGetModuleBase64,
                    NULL);

                if (!result)
                    break;

                symbolInfo->SizeOfStruct = sizeof(SYMBOL_INFO);
                symbolInfo->MaxNameLen = MAX_SYM_NAME;
                SymFromAddr(process, (ULONG64)stack.AddrPC.Offset, &displacement, symbolInfo);

                HMODULE hModule = NULL;
                lstrcpyA(module, "");
                GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, (const wchar_t*)(stack.AddrPC.Offset), &hModule);

                if (hModule != NULL)
                    GetModuleFileNameA(hModule, module, 256);

                printf("[%lu] Name: %s - Address: %p  - Module: %s\n", frame, symbolInfo->Name, (void*)symbolInfo->Address, module);
            }
        }

        template <ExceptionMode mode>
        auto MemcuryGlobalHandler(EXCEPTION_POINTERS* ExceptionInfo) -> long
        {
            auto [dllStart, dllEnd] = Util::GetModuleStartAndEnd();

            if constexpr (mode == ExceptionMode::CatchDllExceptionsOnly)
            {
                if (!Util::IsInRange(ExceptionInfo->ContextRecord->Rip, dllStart, dllEnd))
                {
                    return EXCEPTION_CONTINUE_SEARCH;
                }
            }

            auto message = std::format("Memcury caught an exception at [{:x}]\nPress Yes if you want the address to be copied to your clipboard", ExceptionInfo->ContextRecord->Rip);
            if (MessageBoxA(nullptr, message.c_str(), "Error", MB_ICONERROR | MB_YESNO) == IDYES)
            {
                std::string clip = std::format("{:x}", ExceptionInfo->ContextRecord->Rip);
                Util::CopyToClipboard(clip);
            }

            PrintStack(ExceptionInfo->ContextRecord);

            FreezeCurrentThread();

            return EXCEPTION_EXECUTE_HANDLER;
        }

        template <ExceptionMode mode>
        static auto SetExceptionMode() -> void
        {
            SetUnhandledExceptionFilter(MemcuryGlobalHandler<mode>);
        }
    }

    namespace Globals
    {
        constexpr const bool bLogging = true;

        inline const char* moduleName = nullptr;
    }

    namespace ASM
    {
        //@todo: this whole namespace needs a rework, should somehow make this more modern and less ugly.
        enum MNEMONIC : uint8_t
        {
            JMP_REL8 = 0xEB,
            JMP_REL32 = 0xE9,
            JMP_EAX = 0xE0,
            CALL = 0xE8,
            LEA = 0x8D,
            CDQ = 0x99,
            CMOVL = 0x4C,
            CMOVS = 0x48,
            CMOVNS = 0x49,
            NOP = 0x90,
            INT3 = 0xCC,
            RETN_REL8 = 0xC2,
            RETN = 0xC3,
            NONE = 0x00
        };

        constexpr int SIZE_OF_JMP_RELATIVE_INSTRUCTION = 5;
        constexpr int SIZE_OF_JMP_ABSLOUTE_INSTRUCTION = 13;

        constexpr auto MnemonicToString(MNEMONIC e) -> const char*
        {
            switch (e)
            {
            case JMP_REL8:
                return "JMP_REL8";
            case JMP_REL32:
                return "JMP_REL32";
            case JMP_EAX:
                return "JMP_EAX";
            case CALL:
                return "CALL";
            case LEA:
                return "LEA";
            case CDQ:
                return "CDQ";
            case CMOVL:
                return "CMOVL";
            case CMOVS:
                return "CMOVS";
            case CMOVNS:
                return "CMOVNS";
            case NOP:
                return "NOP";
            case INT3:
                return "INT3";
            case RETN_REL8:
                return "RETN_REL8";
            case RETN:
                return "RETN";
            case NONE:
                return "NONE";
            default:
                return "UNKNOWN";
            }
        }

        constexpr auto Mnemonic(const char* s) -> MNEMONIC
        {
            switch (Util::StrHash(s))
            {
            case Util::StrHash("JMP_REL8"):
                return JMP_REL8;
            case Util::StrHash("JMP_REL32"):
                return JMP_REL32;
            case Util::StrHash("JMP_EAX"):
                return JMP_EAX;
            case Util::StrHash("CALL"):
                return CALL;
            case Util::StrHash("LEA"):
                return LEA;
            case Util::StrHash("CDQ"):
                return CDQ;
            case Util::StrHash("CMOVL"):
                return CMOVL;
            case Util::StrHash("CMOVS"):
                return CMOVS;
            case Util::StrHash("CMOVNS"):
                return CMOVNS;
            case Util::StrHash("NOP"):
                return NOP;
            case Util::StrHash("INT3"):
                return INT3;
            case Util::StrHash("RETN_REL8"):
                return RETN_REL8;
            case Util::StrHash("RETN"):
                return RETN;
            default:
                return NONE;
            }
        }

        inline auto byteIsA(uint8_t byte, MNEMONIC opcode) -> bool
        {
            return byte == opcode;
        }

        inline auto byteIsAscii(uint8_t byte) -> bool
        {
            static constexpr bool isAscii[0x100] = {
                false, false, false, false, false, false, false, false, false, true, true, false, false, true, false, false,
                false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
                true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true,
                true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true,
                true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true,
                true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true,
                true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true,
                true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, false,
                false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
                false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
                false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
                false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
                false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
                false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
                false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
                false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false
            };

            return isAscii[byte];
        }

        inline bool isJump(uint8_t byte)
        {
            return byte >= 0x70 && byte <= 0x7F;
        }

        static auto pattern2bytes(const char* pattern) -> std::vector<int>
        {
            auto bytes = std::vector<int>{};
            const auto start = const_cast<char*>(pattern);
            const auto end = const_cast<char*>(pattern) + strlen(pattern);

            for (auto current = start; current < end; ++current)
            {
                if (*current == '?')
                {
                    ++current;
                    if (*current == '?')
                        ++current;
                    bytes.push_back(-1);
                }
                else
                {
                    bytes.push_back(strtoul(current, &current, 16));
                }
            }
            return bytes;
        }

        static bool IsFunctionEnd(uint8_t* Address)
        {
            int Align = 0x10 - (uintptr_t(Address) % 0x10);

            return Address[0] == 0xC3 &&
                Address[Align] == 0x40 &&
                ((Address[-1] >= 0x58 && Address[-1] <= 0x5F) ||
                    (Address[-2] == 0x41 && Address[-1] >= 0x58 && Address[-1] <= 0x5F));
        }
    }

    namespace PE
    {
        inline auto SetCurrentModule(const char* moduleName) -> void
        {
            Globals::moduleName = moduleName;
        }

        inline auto GetModuleBase() -> uintptr_t
        {
            return reinterpret_cast<uintptr_t>(GetModuleHandleA(Globals::moduleName));
        }

        inline auto GetDOSHeader() -> PIMAGE_DOS_HEADER
        {
            return reinterpret_cast<PIMAGE_DOS_HEADER>(GetModuleBase());
        }

        inline auto GetNTHeaders() -> PIMAGE_NT_HEADERS
        {
            return reinterpret_cast<PIMAGE_NT_HEADERS>(GetModuleBase() + GetDOSHeader()->e_lfanew);
        }

        class Address
        {
            uintptr_t _address;

        public:
            Address()
            {
                _address = 0;
            }

            Address(uintptr_t address)
                : _address(address)
            {
            }

            Address(void* address)
                : _address(reinterpret_cast<uintptr_t>(address))
            {
            }

            auto operator=(uintptr_t address) -> Address
            {
                _address = address;
                return *this;
            }

            auto operator=(void* address) -> Address
            {
                _address = reinterpret_cast<uintptr_t>(address);
                return *this;
            }

            auto operator+(uintptr_t offset) -> Address
            {
                return Address(_address + offset);
            }

            bool operator>(uintptr_t offset)
            {
                return _address > offset;
            }

            bool operator>(Address address)
            {
                return _address > address._address;
            }

            bool operator<(uintptr_t offset)
            {
                return _address < offset;
            }

            bool operator<(Address address)
            {
                return _address < address._address;
            }

            bool operator>=(uintptr_t offset)
            {
                return _address >= offset;
            }

            bool operator>=(Address address)
            {
                return _address >= address._address;
            }

            bool operator<=(uintptr_t offset)
            {
                return _address <= offset;
            }

            bool operator<=(Address address)
            {
                return _address <= address._address;
            }

            bool operator==(uintptr_t offset)
            {
                return _address == offset;
            }

            bool operator==(Address address)
            {
                return _address == address._address;
            }

            bool operator!=(uintptr_t offset)
            {
                return _address != offset;
            }

            bool operator!=(Address address)
            {
                return _address != address._address;
            }

            auto RelativeOffset(uint32_t offset) -> Address
            {
                if (!_address)
                    return *this;

                _address = ((_address + offset + 4) + *(int32_t*)(_address + offset));
                return *this;
            }

            auto AbsoluteOffset(uint32_t offset) -> Address
            {
                if (!_address)
                    return *this;

                _address = _address + offset;
                return *this;
            }

            auto Jump() -> Address
            {
                if (!_address)
                    return *this;

                if (ASM::isJump(*reinterpret_cast<UINT8*>(_address)))
                {
                    UINT8 toSkip = *reinterpret_cast<UINT8*>(_address + 1);
                    _address = _address + 2 + toSkip;
                }

                return *this;
            }

            auto Get() -> uintptr_t
            {
                return _address;
            }

            template <typename T>
            auto GetAs() -> T
            {
                return reinterpret_cast<T>(_address);
            }

            auto IsValid() -> bool
            {
                return _address != 0;
            }
        };

        class Section
        {
        public:
            std::string sectionName;
            IMAGE_SECTION_HEADER rawSection;

            static auto GetAllSections() -> std::vector<Section>
            {
                std::vector<Section> sections;

                auto sectionsSize = GetNTHeaders()->FileHeader.NumberOfSections;
                auto section = IMAGE_FIRST_SECTION(GetNTHeaders());

                for (WORD i = 0; i < sectionsSize; i++, section++)
                {
                    auto secName = std::string((char*)section->Name);

                    sections.push_back({ secName, *section });
                }

                return sections;
            }

            static auto GetSection(std::string sectionName) -> Section
            {
                for (auto& section : GetAllSections())
                {
                    if (section.sectionName == sectionName)
                    {
                        return section;
                    }
                }

                MemcuryThrow("Section not found");
                return Section{};
            }

            auto GetSectionSize() -> uint32_t
            {
                return rawSection.Misc.VirtualSize;
            }

            auto GetSectionStart() -> Address
            {
                return Address(GetModuleBase() + rawSection.VirtualAddress);
            }

            auto GetSectionEnd() -> Address
            {
                return Address(GetSectionStart() + GetSectionSize());
            }

            auto isInSection(Address address) -> bool
            {
                return address >= GetSectionStart() && address < GetSectionEnd();
            }
        };
    }

    class Scanner
    {
        PE::Address _address;

    public:
        Scanner(PE::Address address)
            : _address(address)
        {
        }

        static auto SetTargetModule(const char* moduleName) -> void
        {
            PE::SetCurrentModule(moduleName);
        }

        static auto FindPatternEx(HANDLE handle, const char* pattern, const char* mask, uint64_t begin, uint64_t end) -> Scanner
        {
            auto scan = [](const char* pattern, const char* mask, char* begin, unsigned int size) -> char*
                {
                    size_t patternLen = strlen(mask);
                    for (unsigned int i = 0; i < size - patternLen; i++)
                    {
                        bool found = true;
                        for (unsigned int j = 0; j < patternLen; j++)
                        {
                            if (mask[j] != '?' && pattern[j] != *(begin + i + j))
                            {
                                found = false;
                                break;
                            }
                        }

                        if (found)
                            return (begin + i);
                    }
                    return nullptr;
                };

            uint64_t match = NULL;
            SIZE_T bytesRead;
            char* buffer = nullptr;
            MEMORY_BASIC_INFORMATION mbi = { 0 };

            uint64_t curr = begin;

            for (uint64_t curr = begin; curr < end; curr += mbi.RegionSize)
            {
                if (!VirtualQueryEx(handle, (void*)curr, &mbi, sizeof(mbi)))
                    continue;

                if (mbi.State != MEM_COMMIT || mbi.Protect == PAGE_NOACCESS)
                    continue;

                buffer = new char[mbi.RegionSize];

                if (ReadProcessMemory(handle, mbi.BaseAddress, buffer, mbi.RegionSize, &bytesRead))
                {
                    char* internalAddr = scan(pattern, mask, buffer, (unsigned int)bytesRead);

                    if (internalAddr != nullptr)
                    {
                        match = curr + (uint64_t)(internalAddr - buffer);
                        break;
                    }
                }
            }
            delete[] buffer;

            //MemcuryAssertM(match != 0, "FindPatternEx return nullptr");

            return Scanner(match);
        }

        static auto FindPatternEx(HANDLE handle, const char* sig) -> Scanner
        {
            char pattern[100];
            char mask[100];

            char lastChar = ' ';
            unsigned int j = 0;

            for (unsigned int i = 0; i < strlen(sig); i++)
            {
                if ((sig[i] == '?' || sig[i] == '*') && (lastChar != '?' && lastChar != '*'))
                {
                    pattern[j] = mask[j] = '?';
                    j++;
                }

                else if (isspace(lastChar))
                {
                    pattern[j] = lastChar = (char)strtol(&sig[i], 0, 16);
                    mask[j] = 'x';
                    j++;
                }
                lastChar = sig[i];
            }
            pattern[j] = mask[j] = '\0';

            auto module = (uint64_t)GetModuleHandle(nullptr);

            return FindPatternEx(handle, pattern, mask, module, module + Memcury::PE::GetNTHeaders()->OptionalHeader.SizeOfImage);
        }

        static auto FindPattern(const char* signature, bool assert = true, int skip = 0) -> Scanner
        {
            PE::Address add{ nullptr };

            const auto sizeOfImage = PE::GetNTHeaders()->OptionalHeader.SizeOfImage;
            auto patternBytes = ASM::pattern2bytes(signature);
            const auto scanBytes = reinterpret_cast<std::uint8_t*>(PE::GetModuleBase());

            const auto s = patternBytes.size();
            const auto d = patternBytes.data();

            for (auto i = 0ul; i < sizeOfImage - s; ++i)
            {
                bool found = true;
                for (auto j = 0ul; j < s; ++j)
                {
                    if (scanBytes[i + j] != d[j] && d[j] != -1)
                    {
                        found = false;
                        break;
                    }
                }

                if (found)
                {
                    if (skip != 0)
                    {
                        skip--;
                        continue;
                    }

                    add = reinterpret_cast<uintptr_t>(&scanBytes[i]);
                    break;
                }
            }

#ifndef PRODUCTION
           // if (assert)
            //    MemcuryAssertM(add != 0, std::format("FindPattern return nullptr, {}", signature).c_str());
#endif

            return Scanner(add);
        }

        static auto FindAllPatterns(const char* signature, bool assert = true) -> std::vector<Scanner>
        {
            std::vector<Scanner> results;

            const auto sizeOfImage = PE::GetNTHeaders()->OptionalHeader.SizeOfImage;
            auto patternBytes = ASM::pattern2bytes(signature);
            const auto scanBytes = reinterpret_cast<std::uint8_t*>(PE::GetModuleBase());

            const auto s = patternBytes.size();
            const auto d = patternBytes.data();

            for (auto i = 0ul; i < sizeOfImage - s; ++i)
            {
                bool found = true;
                for (auto j = 0ul; j < s; ++j)
                {
                    if (scanBytes[i + j] != d[j] && d[j] != -1)
                    {
                        found = false;
                        break;
                    }
                }

                if (found)
                {
                    results.push_back(Scanner(reinterpret_cast<uintptr_t>(&scanBytes[i])));
                }
            }

            return results;
        }

        static auto FindPatterns(std::vector<const char*> signatures, bool assert = true) -> Scanner
        {
            PE::Address add{ nullptr };

            for (auto signature : signatures)
            {
                const auto sizeOfImage = PE::GetNTHeaders()->OptionalHeader.SizeOfImage;
                auto patternBytes = ASM::pattern2bytes(signature);
                const auto scanBytes = reinterpret_cast<std::uint8_t*>(PE::GetModuleBase());

                const auto s = patternBytes.size();
                const auto d = patternBytes.data();

                for (auto i = 0ul; i < sizeOfImage - s; ++i)
                {
                    bool found = true;
                    for (auto j = 0ul; j < s; ++j)
                    {
                        if (scanBytes[i + j] != d[j] && d[j] != -1)
                        {
                            found = false;
                            break;
                        }
                    }

                    if (found)
                    {
                        add = reinterpret_cast<uintptr_t>(&scanBytes[i]);
                        break;
                    }
                }

                if (add != 0)
                    break;
            }

#ifndef PRODUCTION
            //if (assert)
            //    MemcuryAssertM(add != 0, std::format("FindPatterns return nullptr, {}", signatures[0]).c_str());
#endif

            return Scanner(add);
        }

        static auto FindPointerRef(void* Pointer, int Skip = 0) -> Scanner
        {
            PE::Address add{ nullptr };

            if (!Pointer)
                return Scanner(add);

            auto textSection = PE::Section::GetSection(".text");

            const auto scanBytes = reinterpret_cast<std::uint8_t*>(textSection.GetSectionStart().Get());

            for (DWORD i = 0x0; i < textSection.GetSectionSize(); i++)
            {
                if ((scanBytes[i] == ASM::CMOVL || scanBytes[i] == ASM::CMOVS) && (scanBytes[i + 1] == ASM::LEA || scanBytes[i + 1] == 0x8B))
                {
                    if (PE::Address(&scanBytes[i]).RelativeOffset(3).GetAs<void*>() == Pointer)
                    {
                        add = PE::Address(&scanBytes[i]);

                        if (Skip == 0)
                            break;
                        else
                            Skip--;
                    }
                }

                if (scanBytes[i] == ASM::CALL)
                {
                    if (PE::Address(&scanBytes[i]).RelativeOffset(1).GetAs<void*>() == Pointer)
                    {
                        add = PE::Address(&scanBytes[i]);

                        if (Skip == 0)
                            break;
                        else
                            Skip--;
                    }
                }
            }

            return Scanner(add);
        }

        static auto FindDataPointerRef(void* pointer) -> Scanner
        {
            PE::Address add{ nullptr };

            auto rdataSection = PE::Section::GetSection(".rdata");
            const auto scanBytes = reinterpret_cast<std::uint8_t*>(rdataSection.GetSectionStart().Get());

            for (DWORD i = 0; i < rdataSection.GetSectionSize(); i++)
            {
                auto currentPointer = *reinterpret_cast<void**>(scanBytes + i);

                if (currentPointer == pointer)
                {
                    add = PE::Address(&scanBytes[i]);
                    break;
                }
            }

            return Scanner(add);
        }

        // Supports wide and normal strings both std and pointers
        template <typename T = const wchar_t*>
        static auto FindStringRef(T string, int Skip = 0, bool assert = true) -> Scanner
        {
            PE::Address add{ nullptr };

            constexpr auto bIsWide = std::is_same<T, const wchar_t*>::value;
            constexpr auto bIsChar = std::is_same<T, const char*>::value;

            constexpr auto bIsPtr = bIsWide || bIsChar;

            auto textSection = PE::Section::GetSection(".text");
            auto rdataSection = PE::Section::GetSection(".rdata");

            const auto scanBytes = reinterpret_cast<std::uint8_t*>(textSection.GetSectionStart().Get());

            // scan only text section
            for (DWORD i = 0x0; i < textSection.GetSectionSize(); i++)
            {
                if ((scanBytes[i] == ASM::CMOVL || scanBytes[i] == ASM::CMOVS) && scanBytes[i + 1] == ASM::LEA)
                {
                    auto stringAdd = PE::Address(&scanBytes[i]).RelativeOffset(3);

                    // Check if the string is in the .rdata section
                    if (rdataSection.isInSection(stringAdd))
                    {
                        auto strBytes = stringAdd.GetAs<std::uint8_t*>();
                        auto pointerToRef = *(LPVOID*)strBytes;

                        if (rdataSection.isInSection(pointerToRef)) // I love :3 epic games kawaii so much
                        {
                            strBytes = (std::uint8_t*)pointerToRef;
                            stringAdd = PE::Address(pointerToRef);
                        }

                        // Check if the first char is printable
                        if (ASM::byteIsAscii(strBytes[0]))
                        {
                            if constexpr (!bIsPtr)
                            {
                                typedef T::value_type char_type;

                                auto lea = stringAdd.GetAs<char_type*>();

                                T leaT(lea);

                                if (leaT == string)
                                {
                                    add = PE::Address(&scanBytes[i]);

                                    if (Skip == 0)
                                        break;
                                    else
                                        Skip--;
                                }
                            }
                            else
                            {
                                auto lea = stringAdd.GetAs<T>();

                                if constexpr (bIsWide)
                                {
                                    if (wcscmp(string, lea) == 0)
                                    {
                                        add = PE::Address(&scanBytes[i]);

                                        if (Skip == 0)
                                            break;
                                        else
                                            Skip--;
                                    }
                                }
                                else
                                {
                                    if (strcmp(string, lea) == 0)
                                    {
                                        add = PE::Address(&scanBytes[i]);

                                        if (Skip == 0)
                                            break;
                                        else
                                            Skip--;
                                    }
                                }
                            }
                        }
                    }
                }
            }

#ifndef PRODUCTION
            if (add == 0 && assert)
            {
                if constexpr (bIsWide)
                    MessageBoxW(0, string, L"FindStringRef return nullptr", 0);

                if constexpr (bIsChar)
                    MessageBoxA(0, string, "FindStringRef return nullptr", 0);
            }
#endif

            return Scanner(add);
        }

        auto Jump() -> Scanner
        {
            _address.Jump();
            return *this;
        }

        auto ScanFor(std::vector<uint8_t> opcodesToFind, bool forward = true, int toSkip = 0) -> Scanner
        {
            if (!IsValid())
                return *this;

            const auto scanBytes = _address.GetAs<std::uint8_t*>();

            for (auto i = (forward ? 1 : -1); forward ? (i < 2048) : (i > -2048); forward ? i++ : i--)
            {
                bool found = true;

                for (int k = 0; k < opcodesToFind.size() && found; k++)
                {
                    if (opcodesToFind[k] == -1)
                        continue;
                    found = opcodesToFind[k] == scanBytes[i + k];
                }

                if (found)
                {
                    _address = &scanBytes[i];
                    if (toSkip != 0)
                    {
                        return ScanFor(opcodesToFind, forward, toSkip - 1);
                    }

                    break;
                }
            }

            return *this;
        }

        auto FindFunctionBoundary(bool forward = false) -> Scanner
        {
            const auto scanBytes = _address.GetAs<std::uint8_t*>();

            for (auto i = (forward ? 1 : -1); forward ? (i < 2048) : (i > -2048); forward ? i++ : i--)
            {
                if (ASM::IsFunctionEnd(&scanBytes[i]))
                    return Scanner(&scanBytes[i + 1]);

                if ((scanBytes[i] % 0x10 == 0) && (scanBytes[i] == 0x40 && (scanBytes[i + 1] >= 0x50 && scanBytes[i + 1] <= 0x57) && (scanBytes[i + 2] >= 0x50 && scanBytes[i + 2] <= 0x57)))
                {
                    return Scanner(&scanBytes[i + 1]);
                }
            }
            return *this;
        }

        auto RelativeOffset(uint32_t offset) -> Scanner
        {
            _address.RelativeOffset(offset);

            return *this;
        }

        auto AbsoluteOffset(uint32_t offset) -> Scanner
        {
            _address.AbsoluteOffset(offset);

            return *this;
        }

        template <typename T>
        auto GetAs() -> T
        {
            return _address.GetAs<T>();
        }

        auto Get() -> uintptr_t
        {
            return _address.Get();
        }

        auto IsValid() -> bool
        {
            return _address.IsValid();
        }
    };
}