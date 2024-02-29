#pragma once

// identify operating system and toolchain
#if defined(WIN64) || defined(_WIN64) || defined(WIN32)
#define SR_PLATFORM_WIN64

#elif defined(__linux__)
#define SR_PLATFORM_LINUX

#else
    #error "Platform not supported"
#endif


// dll import/export macros
#if defined(SR_PLATFORM_WIN64)
#if defined(SR_EXPORT_DLL)
#define SR_API __declspec(dllexport)
#else
#define SR_API __declspec(dllimport)
#endif

#else
#define SR_API

#endif