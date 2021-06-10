#pragma once

#include "gaPrerequisitesUtilities.h"

namespace gaEngineSDK {

  class Degrees;

  class GA_UTILITY_EXPORT Radians
  {
    public:
      /***********************************************************************/
      /**
      * Constructor and Destructor.
      */
      /***********************************************************************/

      Radians(const Radians& radian) = default;

      Radians(const float& radian);

      Radians(const Degrees& degree);
    
      ~Radians() = default;

      /***********************************************************************/
      /**
      * Methods.
      */
      /***********************************************************************/

      float
      valueDegrees() const;

      float
      valueRadians() const;

      void
      unwindRadians();

      /***********************************************************************/
      /**
      * Overlaod functions.
      */
      /***********************************************************************/
    
      constexpr Radians&
      operator=(const float& f);

      constexpr Radians&
      operator=(const Radians& radian) = default;

      Radians&
      operator=(const Degrees& degree);

      const Radians&
      operator+() const;

      Radians
      operator+(const Radians& radian) const;

      Radians
      operator+(const Degrees& degree) const;

      Radians&
      operator+=(const Radians& radian);

      Radians&
      operator+=(const Degrees& degree);

      Radians
      operator-() const;

      Radians
      operator-(const Radians& radian) const;

      Radians
      operator-(const Degrees& degree) const;

      Radians&
      operator-=(const Radians& radian);

      Radians&
      operator-=(const Degrees& degree);

      Radians
      operator*(float f) const;

      Radians
      operator*(const Radians& radian) const;

      Radians&
      operator*=(float f);

      Radians&
      operator*=(const Radians& radian);

      Radians
      operator/(float f) const;

      Radians&
      operator/=(float f);

      Radians
      operator/(const Radians& radian) const;

      Radians&
      operator/=(const Radians& radian);

      bool
      operator<(const Radians& radian) const;

      bool
      operator<=(const Radians& radian) const;

      bool
      operator==(const Radians& radian) const;

      bool
      operator!=(const Radians& radian) const;

      bool
      operator>=(const Radians& radian) const;

      bool
      operator>(const Radians & radian) const;

      bool
      operator<(const float& f) const;

      bool
      operator<=(const float& f) const;

      bool
      operator==(const float& f) const;

      bool
      operator!=(const float& f) const;

      bool
      operator>=(const float& f) const;

      bool
      operator>(const float& f) const;

      /***********************************************************************/
      /**
      * Members.
      */
      /***********************************************************************/

      float m_radian;
  };
}