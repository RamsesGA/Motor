#pragma once

#include "gaPrerequisitesUtilities.h"

namespace gaEngineSDK {

  class GA_UTILITY_EXPORT Degrees
  {
    public:
      /*************************************************************************/
      /**
      * Constructor and Destructor
      */
      /*************************************************************************/
      Degrees(float degree);
    
      virtual
      ~Degrees() = default;
    
      float
      degreesToRadians(float degrees);

      float m_degree;
  };
}