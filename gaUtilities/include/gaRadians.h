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
      Radians() = default;
    
      ~Radians() = default;
    
      template<typename type>
      type&
      radiansToDegrees(type radian);
    
    private:
      /*************************************************************************/
      /**
      * Members
      */
      /*************************************************************************/
      float m_pi = 3.141592f;
  };
}