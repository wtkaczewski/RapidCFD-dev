#pragma once

#include <stdio.h>
#include <type_traits>

//#ifdef _​_HIP_​PLATFORM_​AMD_​_
#include <hip/hip_runtime.h>
//#endif

#ifdef __CUDACC__
//#include <hip/hip_runtime.h>
#endif

#define GPU_API_CALL(x) do { if((x) != hipSuccess) {         \
 printf("Error at %s:%d\n",__FILE__,__LINE__);             \
 printf("%s\n",hipGetErrorString(hipPeekAtLastError())); \
 hipDeviceReset();                                        \
 ::exit(static_cast<int>(hipPeekAtLastError()));          \
 }} while(0)

#define GPU_ERROR_CHECK()                                  \
 hipDeviceSynchronize();                                  \
 GPU_API_CALL( hipPeekAtLastError());

#define GPU_ERROR_CHECK_ASYNC()                            \
 GPU_API_CALL(hipPeekAtLastError());

namespace Foam
{
    int deviceCount();
    int currentDevice();
    void setCurrentDevice(int device);
    int deviceComputeCapability(int device);
    int currentComputeCapability();
    bool needTextureBind();
    
    template<class T>
    bool hasAtomicAdd();

    template<class T>
    struct is_number: std::false_type {};
}

#include "DeviceConfigI.H"

