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
    
      ~Radians() = default;
    
      float
      radiansToDegrees(float radian);

      float m_radian;
    
    private:
      /*************************************************************************/
      /**
      * Members
      */
      /*************************************************************************/
      float m_pi = 3.141592f;
  };
}