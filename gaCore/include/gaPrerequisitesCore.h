#pragma once

/*****************************************************************************/
/**
* Includes
*/
/*****************************************************************************/
#include <gaPrerequisitesUtilities.h>

/*****************************************************************************/
/**
* DLL export
*/
/*****************************************************************************/
#if GA_PLATFORM == GA_PLATFORM_WIN32
  #if GA_COMPILER == GA_COMPILER_MSVC
    #if defined (GA_STATIC_LIB)
      #define GA_CORE_EXPORT
    #else
      #if defined (GA_CORE_EXPORTS)
        #define GA_CORE_EXPORT __declspec(dllexport)
      #else
        #define GA_CORE_EXPORT __declspec(dllimport)
      #endif
    #endif
  #else  //Any other compiler
    #if defined(GA_STATIC_LIB)
      #define GA_CORE_EXPORT
    #else
      #if defined(GA_CORE_EXPORTS)
        #define GA_CORE_EXPORT __attribute__((dllexport))
      #else
        #define GA_CORE_EXPORT __attribute__((dllimport))
      #endif
    #endif
  #endif

#define GA_CORE_HIDDEN

#else //Linux-Mac settings
  #define GA_CORE_EXPORT __attribute__((visibility("default")))
  #define GA_CORE_HIDDEN __attribute__((visibility("hidden")))
#endif
