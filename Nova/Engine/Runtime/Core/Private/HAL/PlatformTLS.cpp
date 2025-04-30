#include "HAL/PlatformTLS.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>


uint32 FPlatformTLS::GetCurrentThreadId(void)
{
    return ::GetCurrentThreadId();
}

/**
 * Allocates a thread local store slot.
 *
 * @return The index of the allocated slot.
 */
uint32 FPlatformTLS::AllocTlsSlot(void)
{
    return TlsAlloc();
}

/**
 * Sets a value in the specified TLS slot.
 *
 * @param SlotIndex the TLS index to store it in.
 * @param Value the value to store in the slot.
 */
void FPlatformTLS::SetTlsValue(uint32 SlotIndex, void* Value)
{
    TlsSetValue(SlotIndex, Value);
}

/**
 * Reads the value stored at the specified TLS slot.
 *
 * @param SlotIndex The index of the slot to read.
 * @return The value stored in the slot.
 */
void* FPlatformTLS::GetTlsValue(uint32 SlotIndex)
{
    return TlsGetValue(SlotIndex);
}

/**
 * Frees a previously allocated TLS slot
 *
 * @param SlotIndex the TLS index to store it in
 */
void FPlatformTLS::FreeTlsSlot(uint32 SlotIndex)
{
    TlsFree(SlotIndex);
}