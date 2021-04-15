#pragma once

#include "gaPrerequisitesUtilities.h"

namespace gaEngineSDK {

  class GA_UTILITY_EXPORT Radians
  {
    public:
      /***********************************************************************/
      /**
      * Constructor and Destructor.
      */
      /***********************************************************************/
      Radians(float radians);
    
      ~Radians() = default;

      /***********************************************************************/
      /**
      * Methods.
      */
      /***********************************************************************/
    
      float
      radiansToDegrees(float radian);

      /***********************************************************************/
      /**
      * Members.
      */
      /***********************************************************************/

      float m_radian;
  };
}