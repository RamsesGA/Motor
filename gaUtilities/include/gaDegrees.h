#pragma once

#include "gaPrerequisitesUtilities.h"

namespace gaEngineSDK {
  class Radians;

  class GA_UTILITY_EXPORT Degrees
  {
    public:
      /***********************************************************************/
      /**
      * Constructor and Destructor.
      */
      /***********************************************************************/
      constexpr Degrees(const Degrees& degree) = default;

      constexpr Degrees(float f) : m_degree(f) {}

      Degrees(const Radians& radian);
    
      ~Degrees() = default;

      /***********************************************************************/
      /**
      * Methods.
      */
      /***********************************************************************/

      float
      valueRadians() const;

      void
      unwindDegrees();

      /***********************************************************************/
      /**
      * Overload functions.
      */
      /***********************************************************************/
    
      constexpr Degrees&
      operator=(const Degrees& degree) = default;

      Degrees&
      operator=(const float& f);

      Degrees&
      operator=(const Radians& radian);

      const Degrees&
      operator+() const;

      Degrees
      operator+(const Degrees& degree) const;

      Degrees
      operator+(const Radians& radian) const;

      Degrees&
      operator+=(const Degrees& degree);

      Degrees&
      operator+=(const Radians& radian);

      Degrees
      operator-() const;

      Degrees
      operator-(const Degrees& degree) const;

      Degrees
      operator-(const Radians& radian) const;

      Degrees&
      operator-=(const Degrees& degree);

      Degrees&
      operator-=(const Radians& radian);

      Degrees
      operator*(float f) const;

      Degrees
      operator*(const Degrees& degree) const;

      Degrees&
      operator*=(float f);

      Degrees&
      operator*=(const Degrees& degree);

      Degrees
      operator/(float f) const;

      Degrees&
      operator/=(float f);

      Degrees
      operator/(const Degrees& degree) const;

      Degrees&
      operator/=(const Degrees& degree);

      bool
      operator<(const Degrees& degree) const;

      bool
      operator<=(const Degrees& degree) const;

      bool
      operator==(const Degrees& degree) const;

      bool
      operator!=(const Degrees& degree) const;

      bool
      operator>=(const Degrees& degree) const;

      bool
      operator>(const Degrees& degree) const;

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

      float m_degree;
  };
}