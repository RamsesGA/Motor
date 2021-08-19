#pragma once

#include "gaPrerequisitesUtilities.h"
#include "gaPlatformMath.h"

namespace gaEngineSDK {
  class GA_UTILITY_EXPORT WindowsMath : public PlatformMath 
  {
   public:
    /***********************************************************************/
    /*
    * Constructor and destructor.
    */
    /***********************************************************************/
    WindowsMath() = default;
    
    ~WindowsMath() = default;
  };
}