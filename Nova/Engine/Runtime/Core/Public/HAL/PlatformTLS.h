#pragma once
#include "Platform.h"

struct FPlatformTLS
{
    /**
 * Returns the currently executing thread's identifier.
 *
 * @return The thread identifier.
 */
    static uint32 GetCurrentThreadId();

    /**
     * Allocates a thread local store slot.
     *
     * @return The index of the allocated slot.
     */
    static uint32 AllocTlsSlot();

    /**
     * Sets a value in the specified TLS slot.
     *
     * @param SlotIndex the TLS index to store it in.
     * @param Value the value to store in the slot.
     */
    static void SetTlsValue(uint32 SlotIndex, void* Value);

    /**
     * Reads the value stored at the specified TLS slot.
     *
     * @param SlotIndex The index of the slot to read.
     * @return The value stored in the slot.
     */
    static void* GetTlsValue(uint32 SlotIndex);

    /**
     * Frees a previously allocated TLS slot
     *
     * @param SlotIndex the TLS index to store it in
     */
    static void FreeTlsSlot(uint32 SlotIndex);
};
