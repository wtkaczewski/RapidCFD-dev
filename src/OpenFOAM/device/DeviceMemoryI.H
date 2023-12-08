#pragma once

#include "DeviceConfig.H"

#include <thrust/device_ptr.h>
#include <thrust/device_malloc.h>
#include <thrust/device_free.h>

template<class T>
inline T* Foam::allocDevice(const label size)
{
    using namespace thrust;
    return raw_pointer_cast(device_malloc<T>(size));
}

template<class T>
inline void Foam::freeDevice(T* ptr)
{
    using namespace thrust;
    device_free(device_pointer_cast(ptr));
}


template<class T>
inline T* Foam::allocPageLocked(const label size)
{
    T* ptr;
    GPU_API_CALL(hipHostMalloc(&ptr, size*sizeof(T)));
    return ptr;
}

template<class T>
inline void Foam::freePageLocked(T* ptr)
{
    GPU_API_CALL(hipHostFree(ptr));
}


inline void Foam::copyHostToDevice(void* dst, const void* src, const label size)
{
    GPU_API_CALL(hipMemcpy(dst, src, size, hipMemcpyHostToDevice));
}


inline void Foam::copyDeviceToHost(void* dst, const void* src, const label size)
{
    GPU_API_CALL(hipMemcpy(dst, src, size, hipMemcpyDeviceToHost));
}


inline void Foam::copyDeviceToDevice(void* dst, const void* src, const label size)
{
    GPU_API_CALL(hipMemcpy(dst, src, size, hipMemcpyDeviceToDevice));
}

