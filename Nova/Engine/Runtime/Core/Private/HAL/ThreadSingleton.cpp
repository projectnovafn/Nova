#include "HAL/ThreadSingleton.h"

#undef InterlockedCompareExchange

/*-----------------------------------------------------------------------------
    FThreadSingletonInitializer
-----------------------------------------------------------------------------*/

FTlsAutoCleanup* FThreadSingletonInitializer::Get(TFunctionRef<FTlsAutoCleanup* ()> CreateInstance, uint32& TlsSlot)
{
    if (TlsSlot == 0xFFFFFFFF)
    {
        const uint32 ThisTlsSlot = FPlatformTLS::AllocTlsSlot();
        const uint32 PrevTlsSlot = FPlatformAtomics::InterlockedCompareExchange((int32*)&TlsSlot, (int32)ThisTlsSlot, 0xFFFFFFFF);
        if (PrevTlsSlot != 0xFFFFFFFF)
        {
            FPlatformTLS::FreeTlsSlot(ThisTlsSlot);
        }
    }
    FTlsAutoCleanup* ThreadSingleton = (FTlsAutoCleanup*)FPlatformTLS::GetTlsValue(TlsSlot);
    if (!ThreadSingleton)
    {
        ThreadSingleton = CreateInstance();
        ThreadSingleton->Register();
        FPlatformTLS::SetTlsValue(TlsSlot, ThreadSingleton);
    }
    return ThreadSingleton;
}

void FTlsAutoCleanup::Register()
{
    /*FRunnableThread* RunnableThread = FRunnableThread::GetRunnableThread();
    if( RunnableThread )
    {
        RunnableThread->TlsInstances.Add( this );
    }*/
}