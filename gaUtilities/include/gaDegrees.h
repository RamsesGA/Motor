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
    
      ~Degrees() = default;
    
      float
      degreesToRadians(float degrees);

      float m_degree;
    
    private:
      /*************************************************************************/
      /**
      * Members
      */
      /*************************************************************************/
      float m_pi = 3.141592f;
  };
}