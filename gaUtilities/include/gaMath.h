#pragma once

#include "gaPrerequisitesUtilities.h"
#include "gaPlatformMath.h"

#if GA_PLATFORM == GA_PLATFORM_WIN32
#include "gaWindowsMath.h"
#elif GA_PLATFORM = GA_PLATFORM_LINUX
#include "gaLinuxMath.h"
#endif

namespace gaEngineSDK {
  #if GA_PLATFORM == GA_PLATFORM_WIN32
    using Math = WindowsMath;
  #elif GA_PLATFORM = GA_PLATFORM_LINUX
    using Math = LinuxMath;
  #else
    using Math = PlatformMath;
  #endif
}