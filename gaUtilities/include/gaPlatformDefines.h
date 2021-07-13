#pragma once

#define GA_PLATFORM_WIN32 1  //Windos plataform
#define GA_PLATFORM_LINUX 2  //Linux plataform

#define GA_COMPILER_MSVC 1  //
#define GA_COMPILER_GNUC 2  //
#define GA_COMPILER_INTEL 3  //
#define GA_COMPILER_CLANG 4  //

#define GA_ARCHITECTURE_X86_32 1  //
#define GA_ARCHITECTURE_X86_64 2  //

#define GA_ENDIAN_LITTLE 1  //
#define GA_ENDIAN_BIG 2  //

#define GA_VERSION_MAJOR 0  //
#define GA_VERSION_MINOR 1  //
#define GA_VERSION_PATCH 0  //

//Define the actual endian type (little endian for Windows, Linux).
#define GA_ENDINA GA_ENDIAN_LITTLE

//Compiler type and version.
#if defined(__clang__)
#define GA_COMPILER GA_COMPILER_CLANG
#define GA_COMP_VER __clang_version__
#define GA_THREADLOCAL __thread
#define GA_STDCALL __attribute__((stdcall))
#define GA_CDECL __attribute__((cdecl))
#define GA_FALLTHROUGH [[clang::fallthrough]]
#elif defined(__GNUC__)  //Check after Clang, as Clang defines this too
#define GA_COMPILER GA_COMPILER_GNUC
#define GA_COMP_VER (((__GNUC__)*100) + (__GNUC_MINOR__*10) + __GNUC_PATCHLEVEL__)
#define GA_THREADLOCAL __thread
#define GA_STDCALL __attribute__((stdcall))
#define GA_CDECL __attribute__((cdecl))
#define GA_FALLTHROUGH __attribute__((fallthrough));
#elif defined(__INTEL_COMPILER)
#define GA_COMPILER GA_COMPILER_INTEL
#define GA_COMP_VER __INTEL_COMPILER
#define GA_STDCALL __stdcall
#define GA_CDECL __cdecl
#define GA_FALLTHROUGH

/**
* GA_THREADLOCAL define is down below because Intel compiler defines it
* differently based on plataform
*/

//Check after Clang and Intel, we could be building with either with VisualStudio
#elif defined(_MSC_VER)
#define GA_COMPILER GA_COMPILER_MSVC
#define GA_COMP_VER _MSC_VER
#define GA_THREADLOCAL __declspec(thread)
#define GA_STDCALL __stdcall
#define GA_CDECL __cdecl
#define GA_FALLTHROUGH
#undef __PRETTY_FUNCTION__
#define __PRETTY_FUNCTION__ __FUNCSIG__
#else
//No know compiler found, send the error to the output (if any)
#pragma error "No known compiler."
#endif

/**
* See if we can use __forceinline or if we need to use __inline instead
*/
#if GA_COMPILER == GA_COMPILER_MSVC  //If we are compiling on VS
  #if GA_COMP_VR >= 1200  //If we are on VS 6 or higher
    #define FORCEINLINE __forceinline  //Set __forceinline
    #ifndef RESTRICT
      #define RESTRICT __restrict //No alias hint
  #endif
#endif
#elif defined(__MINGW32__)  //If we are on Unix type system
  #if !defined(FORCEINLINE)
    #define FORCEINLINE __inline  //Set __inline
    #ifndef RESTRICT
      #define RESTRICT  //No alias hint
    #endif
  #endif
#else //Any other compiler
  #define FORCEINLINE __inline  //Set __inline
  #ifndef RESTRICT
    #define RESTRICT __restrict  //No alias hint
  #endif
#endif

/**
* Finds the current platform
*/
#if defined(__WIN32__)  || defined(_WIN32)  //If it´s a Windows platform
#define GA_PLATFORM GA_PLATFORM_WIN32
#else  //Will consider it as a Linux platform
#define GA_PLATFORM GA_PLATFORM_LINUX
#endif

/**
* Find the architecture type
*/
#if defined(__x86_64__) || defined(_M_X64)  //If it´s a x64 compile
#define GA_ARCH_TYPE GA_ARCHITECTURE_X86_64
#else  //If it´s a x86 compile
#define GA_ARCH_TYPE GA_ARCHITECTURE_X86_32
#endif

/**
* Memory Alignment macros
*/
#if GA_COMPILER == GA_COMPILER_MSVC  //If we are compiling on VS
  #define MS_ALIGN(n) __declspec(align(n))
  #ifndef GCC_PACK
    #define GCC_PACK(n)
  #endif
  #ifndef GCC_ALIGN
    #define GCC_ALIGN(n)
  #endif
#elif (GA_COMPILER == GA_COMPILER_GNUC)
  #define MS_ALIGN(n)
  #define GCC_PACK(n)
  #define GCC_ALIGN(n) __attribute__((__aligned__(n)))
#else  //If we are on a Unix type system
  #define MS_ALIGN(n)
  #define GCC_PACK(n) __attribute__((packed, aligned(n)))
  #define GCC_ALIGN(n) __attribute__((aligned(n)))
#endif

/**
* For throw override
*/
#if GA_COMPILER == GA_COMPILER_MSVC ||                                        \
    GA_COMPILER == GA_COMPILER_INTEL ||                                       \
    GA_COMPILER == GA_COMPILER_GNUC
  #define _NOEXCEPT noexcept
#else
  #define _NOEXCEPT
#endif

/**
* Library export specifics
*/
#if GA_PLATFORM == GA_PLATFORM_WIN32
  #if GA_COMPILER == GA_COMPILER_MSVC
    #if defined (GA_STATIC_LIB)
      #define GA_UTILITY_EXPORT
    #else
      #if defined (GA_UTILITY_EXPORTS)
        #define GA_UTILITY_EXPORT __declspec(dllexport)
      #else
        #define GA_UTILITY_EXPORT __declspec(dllimport)
      #endif
    #endif
  #else  //Any other compiler
    #if defined(GA_STATIC_LIB)
      #define GA_UTILITY_EXPORT
    #else
      #if defined(GA_UTILITY_EXPORTS)
        #define GA_UTILITY_EXPORT __attribute__((dllexport))
      #else
        #define GA_UTILITY_EXPORT __attribute__((dllimport))
      #endif
    #endif
  #endif
  #define GA_UTILITY_HIDDEN
#else //Linux-Mac settings
  #define GA_UTILITY_EXPORT __attribute__((visibility("default")))
  #define GA_UTILITY_HIDDEN __attribute__((visibility("hidden")))
#endif

/**
* DLL export for plug ins
*/
#if GA_PLATFORM == GA_PLATFORM_WIN32
  #if GA_COMPILER == GA_COMPILER_MSVC
    #define GA_PLUGIN_EXPORT __declspec(dllexport)
  #endif
#else  //Linux-Mac setting
  #define GA_PLUGIN_EXPORT __attribute__ ((visibility("default")))
#endif

/**
* Windows specific settings, win32 compilers use _DEBUG 
* for specifying debug builds. For MinGW, we set DEBUG.
*/
#if GA_PLATFORM == GA_PLATFORM_WIN32
  #if defined(_DEBUG) || defined(DEBUG)
    #define GA_DEBUG_MODE 1  //Specifies that we are on a DEBUG build
  #else
    #define GA_DEBUG_MODE 0  //We aren+t on a DEBUG build
  #endif
  #if GA_COMPILER == GA_COMPILER_INTEL
    #define GA_THREADLOCAL __declspec(thread)  //Set the local thread for the Intel compiler
  #endif
#endif  //GA_PLATFORM == GA_PLATFORM_WIN32

/**
* Linux-Apple specific settings
*/
#if GA_PLATFORM == GA_PLATFORM_LINUX || GA_PLATFORM == GA_PLATFORM_OSX
  #define stricmp strcasecmp

//If we are on a DEBUG build
  #if defined(_DEBUG) || defined(DEBUG)
    #define GA_DEBUG_MODE 1  //Specifies that we are on a DEBUG build
  #else
    #define GA_DEBUG_MODE 0  //We are not on a DEBUG build
  #endif
  #if GA_COMPILER == GA_COMPILER_INTEL
    #define GA_THREADLOCAL __thread  //Set the local thread for the Intel compiler
  #endif
#endif  //GA_PLATFORM == GA_PLATFORM_LINUX || GA_PLATFORM == GA_PLATFORM_OSX

/**
* Definition of DEBUG macros
*/
#if GA_DEBUG_MODE
  #define GA_DEBUG_ONLY(x) x
  #define GA_ASSERT(x) assert(x)
#else
  #define GA_DEBUG_ONLY(x)
  #define GA_ASSERT(x)
#endif

#ifndef GA_UNREFERENCED_PARAMETER
  # define GA_UNREFERENCED_PARAMETER(P) (void)P
#endif