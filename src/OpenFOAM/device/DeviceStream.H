#pragma once

#include "DeviceConfig.H"

namespace Foam {

class DeviceStream {
    hipStream_t  stream_;

public:
    DeviceStream();
    ~DeviceStream();

    void synchronize() const;
    hipStream_t operator()() const;
};

}

#include "DeviceStreamI.H"

