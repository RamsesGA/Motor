#pragma once

#include "gaPrerequisitesUtilities.h"

namespace gaEngineSDK {

  class GA_UTILITY_EXPORT Radians
  {
    public:
      /*************************************************************************/
      /**
      * Constructor and Destructor
      */
      /*************************************************************************/
      Radians(float radians);
    
      virtual
      ~Radians() = default;
    
      float
      radiansToDegrees(float radian);

      float m_radian;
  };
}