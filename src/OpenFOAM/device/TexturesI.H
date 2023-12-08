#pragma once

namespace Foam
{

#ifdef __HIP_ARCH_HAS_WARP_FUNNEL_SHIFT__

template<class T>
inline __device__ T textures<T>::operator[](const int& i) const
{
    return __ldg(data + i);
}

#else

template<>
inline __device__ int textures<int>::operator[](const int& i) const
{
    return tex1Dfetch<int>(tex, i);
}

template<>
inline __device__ long textures<long>::operator[](const int& i) const
{
    int2 v = tex1Dfetch<int2>(tex, i);
    long res = v.y;
    res = res << 32;
    return res | v.x;
}

template<>
inline __device__ float textures<float>::operator[](const int& i) const
{
    return tex1Dfetch<float>(tex, i);
}

template<>
inline __device__ double textures<double>::operator[](const int& i) const
{
    int2 v = tex1Dfetch<int2>(tex, i);
    return __hiloint2double(v.y, v.x);
}

#endif

template<class T>
inline void textureBind<T>::bind(int n)
{
    hipResourceDesc resDesc;
    memset(&resDesc, 0, sizeof(hipResourceDesc));

    resDesc.resType = hipResourceTypeLinear;
    resDesc.res.linear.devPtr = const_cast<T*>(data);
    resDesc.res.linear.sizeInBytes = n*sizeof(T);

    initResourceDesc(resDesc);

    hipTextureDesc texDesc;
    memset(&texDesc, 0, sizeof(hipTextureDesc));
    texDesc.readMode = hipReadModeElementType;

    hipCreateTextureObject(&tex, &resDesc, &texDesc, NULL);
}

template<>
inline void textureBind<int>::initResourceDesc(hipResourceDesc& resDesc)
{
    resDesc.res.linear.desc.f = hipChannelFormatKindSigned;
    resDesc.res.linear.desc.x = 32;
}

template<>
inline void textureBind<long>::initResourceDesc(hipResourceDesc& resDesc)
{
    resDesc.res.linear.desc.f = hipChannelFormatKindSigned;
    resDesc.res.linear.desc.x = 32;
    resDesc.res.linear.desc.y = 32;
}

template<>
inline void textureBind<float>::initResourceDesc(hipResourceDesc& resDesc)
{
    resDesc.res.linear.desc.f = hipChannelFormatKindFloat;
    resDesc.res.linear.desc.x = 32;
}

template<>
inline void textureBind<double>::initResourceDesc(hipResourceDesc& resDesc)
{
    resDesc.res.linear.desc.f = hipChannelFormatKindSigned;
    resDesc.res.linear.desc.x = 32;
    resDesc.res.linear.desc.y = 32;
}

template<class T>
inline void textureBind<T>::initResourceDesc(hipResourceDesc& resDesc)
{
    static_assert(is_number<T>::value, "Only numbers can be used as texture resources");
}


}
