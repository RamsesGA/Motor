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
      Degrees() = default;
    
      ~Degrees() = default;
    
      template<typename type>
      type&
      degreesToRadians(type degrees);
    
    private:
      /*************************************************************************/
      /**
      * Members
      */
      /*************************************************************************/
      float m_pi = 3.141592f;
  };
}