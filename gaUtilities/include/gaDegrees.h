#pragma once

#include "gaPrerequisitesUtilities.h"

namespace gaEngineSDK {

  class GA_UTILITY_EXPORT Degrees
  {
    public:
      /***********************************************************************/
      /**
      * Constructor and Destructor.
      */
      /***********************************************************************/
      Degrees(float degree);
    
      ~Degrees() = default;

      /***********************************************************************/
      /**
      * Methods.
      */
      /***********************************************************************/
    
      float
      degreesToRadians(float degrees);

      /***********************************************************************/
      /**
      * Members.
      */
      /***********************************************************************/

      float m_degree;
  };
}