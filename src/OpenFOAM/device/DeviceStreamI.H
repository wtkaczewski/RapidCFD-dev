

inline Foam::DeviceStream::DeviceStream()
{
    GPU_API_CALL(hipStreamCreate(&stream_));
}


inline Foam::DeviceStream::~DeviceStream()
{
    GPU_API_CALL(hipStreamDestroy(stream_));
}


inline void Foam::DeviceStream::synchronize() const
{
    GPU_API_CALL(hipStreamSynchronize(stream_));
}

inline hipStream_t Foam::DeviceStream::operator()() const
{
    return stream_;
}
