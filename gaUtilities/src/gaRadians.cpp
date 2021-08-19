#include "gaRadians.h"
#include "gaMath.h"
#include "gaDegrees.h"

namespace gaEngineSDK {
  Radians::Radians(const float& radian) {
    m_radian = radian;
  }
  
  Radians::Radians(const Degrees& degree) {
    m_radian = degree.valueDegrees();
  }
  
  float
  Radians::valueDegrees() const {
    return m_radian * Math::RAD2DEG;
  }
  
  float
  Radians::valueRadians() const {
    return m_radian;
  }
  
  void
  Radians::unwindRadians() {
    m_radian = Math::unwindRadians(m_radian);
  }
  
  constexpr
  Radians& Radians::operator=(const float& f) {
    m_radian = f;
    return *this;
  }
  
  Radians&
  Radians::operator=(const Degrees& degree) {
    m_radian = degree.valueRadians();
    return *this;
  }
  
  const
  Radians& Radians::operator+() const {
    return *this;
  }
  
  Radians
  Radians::operator+(const Radians& radian) const {
    return Radians(m_radian + radian.m_radian);
  }
  
  Radians
  Radians::operator+(const Degrees& degree) const {
    return Radians(m_radian + degree.valueRadians());
  }
  
  Radians&
  Radians::operator+=(const Radians& radian) {
    m_radian += radian.m_radian;
    return *this;
  }
  
  Radians&
  Radians::operator+=(const Degrees& degree) {
    m_radian += degree.valueRadians();
    return *this;
  }
  
  Radians
  Radians::operator-() const {
    return Radians(-m_radian);
  }
  
  Radians
  Radians::operator-(const Radians& radian) const {
    return Radians(m_radian - radian.m_radian);
  }
  
  Radians
  Radians::operator-(const Degrees& degree) const {
    return Radians(m_radian - degree.valueRadians());
  }
  
  Radians&
  Radians::operator-=(const Radians& radian) {
    m_radian -= radian.m_radian;
    return *this;
  }
  
  Radians&
  Radians::operator-=(const Degrees& degree) {
    m_radian -= degree.valueRadians();
    return *this;
  }
  
  Radians
  Radians::operator*(float f) const {
    return Radians(m_radian * f);
  }
  
  Radians
  Radians::operator*(const Radians& radian) const {
    return Radians(m_radian * radian.m_radian);
  }
  
  Radians&
  Radians::operator*=(float f) {
    m_radian *= f;
    return *this;
  }
  
  Radians&
  Radians::operator*=(const Radians& radian) {
    m_radian *= radian.m_radian;
    return *this;
  }
  
  Radians
  Radians::operator/(float f) const {
    return Radians(m_radian / f);
  }
  
  Radians&
  Radians::operator/=(float f) {
    m_radian /= f;
    return *this;
  }
  
  Radians
  Radians::operator/(const Radians& radian) const {
    return Radians(m_radian / radian.m_radian);
  }
  
  Radians&
  Radians::operator/=(const Radians& radian) {
    m_radian /= radian.m_radian;
    return *this;
  }
  
  bool
  Radians::operator<(const Radians& radian) const {
    return m_radian < radian.m_radian;
  }
  
  bool
  Radians::operator<=(const Radians& radian) const {
    return m_radian <= radian.m_radian;
  }
  
  bool
  Radians::operator==(const Radians& radian) const {
    return m_radian == radian.m_radian;
  }
  
  bool
  Radians::operator!=(const Radians& radian) const {
    return m_radian != radian.m_radian;
  }
  
  bool
  Radians::operator>=(const Radians& radian) const {
    return m_radian >= radian.m_radian;
  }
  
  bool
  Radians::operator>(const Radians& radian) const {
    return m_radian > radian.m_radian;
  }
  
  bool
  Radians::operator<(const float& f) const {
    return m_radian < f;
  }
  
  bool
  Radians::operator<=(const float& f) const {
    return m_radian <= f;
  }
  
  bool
  Radians::operator==(const float& f) const {
    return m_radian == f;
  }
  
  bool
  Radians::operator!=(const float& f) const {
    return m_radian != f;
  }
  
  bool
  Radians::operator>=(const float& f) const {
    return m_radian >= f;
  }
  
  bool
  Radians::operator>(const float& f) const {
    return m_radian > f;
  }
}