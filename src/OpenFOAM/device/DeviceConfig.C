#include "DeviceConfig.H"

namespace Foam {

    int deviceCount()
    {
        int num_devices;
        GPU_API_CALL(hipGetDeviceCount(&num_devices));
        return num_devices;
    }

    int currentDevice()
    {
        int device;
        GPU_API_CALL(hipGetDevice(&device));
        return device;
    }

    void setCurrentDevice(int device)
    {
        GPU_API_CALL(hipSetDevice(device));
    }

    int deviceComputeCapability(int device)
    {
        #ifdef __HIP_PLATFORM_NVIDIA__
            cudaDeviceProp deviceProp;
            GPU_API_CALL(cudaGetDeviceProperties(&deviceProp, device));
            return 10*deviceProp.major + deviceProp.minor;
       #else
            return 0;
       #endif
    }

    int currentComputeCapability()
    {
        return deviceComputeCapability(currentDevice());
    }

    bool needTextureBind()
    {
        #ifdef __HIP_PLATFORM_NVIDIA__
            static bool needBind = currentComputeCapability() < 35;
            return needBind;
       #else
           return true;
      #endif
    }

}
