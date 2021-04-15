#pragma once

#include "gaPrerequisitesUtilities.h"
#include "gaPlatformMath.h"

namespace gaEngineSDK {
  class GA_UTILITY_EXPORT LinuxMath : public PlatformMath 
  {
    public:
      /***********************************************************************/
      /*
      * Constructor and destructor.
      */
      /***********************************************************************/

      LinuxMath() = default;

      ~LinuxMath() = default;
  };
}