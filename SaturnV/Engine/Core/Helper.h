#ifndef HELPER_H
#define HELPER_H

#define WIN32_LEAN_AND_MEAN
#include <Windows.h> // For HRESULT
#include <exception>

// From DXSampleHelper.h 
// Source: https://github.com/Microsoft/DirectX-Graphics-Samples
inline void ThrowIfFailed(HRESULT hr)
{
    if (FAILED(hr))
    {
        throw std::exception();
    }
}

#endif HELPER_H