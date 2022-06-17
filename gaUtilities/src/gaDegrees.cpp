#include "gaDegrees.h"
#include "gaMath.h"
#include "gaRadians.h"

namespace gaEngineSDK {
  Degrees::Degrees(const Radians& radian) {
    m_degree = radian.valueDegrees();
  }
  
  float
  Degrees::valueRadians() const {
    return m_degree * Math::DEG2RAD;
  }
  
  float
  Degrees::valueDegrees() const {
    return m_degree;
  }
  
  void
  Degrees::unwindDegrees() {
    m_degree = Math::unwindDegrees(m_degree);
  }
  
  Degrees&
  Degrees::operator=(const float& f) {
    m_degree = f;
    return *this;
  }
  
  Degrees&
  Degrees::operator=(const Radians& radian) {
    m_degree = radian.valueDegrees();
    return *this;
  }
  
  const
  Degrees& Degrees::operator+() const {
    return *this;
  }
  
  Degrees
  Degrees::operator+(const Degrees& degree) const {
    return Degrees(m_degree + degree.m_degree);
  }
  
  Degrees
  Degrees::operator+(const Radians& radian) const {
    return Degrees(m_degree + radian.valueDegrees());
  }
  
  Degrees&
  Degrees::operator+=(const Degrees& degree) {
    m_degree += degree.m_degree;
    return *this;
  }
  
  Degrees&
  Degrees::operator+=(const Radians& radian) {
    m_degree += radian.valueDegrees();
    return *this;
  }
  
  Degrees
  Degrees::operator-() const {
    return Degrees(-m_degree);
  }
  
  Degrees
  Degrees::operator-(const Degrees& degree) const {
    return Degrees(m_degree - degree.m_degree);
  }
  
  Degrees
  Degrees::operator-(const Radians& radian) const {
    return Degrees(m_degree - radian.valueDegrees());
  }
  
  Degrees&
  Degrees::operator-=(const Degrees& degree) {
    m_degree -= degree.m_degree;
    return *this;
  }
  
  Degrees&
  Degrees::operator-=(const Radians& radian) {
    m_degree -= radian.valueDegrees();
    return *this;
  }
  
  Degrees
  Degrees::operator*(float f) const {
    return Degrees(m_degree * f);
  }
  
  Degrees
  Degrees::operator*(const Degrees& degree) const {
    return Degrees(m_degree * degree.m_degree);
  }
  
  Degrees&
  Degrees::operator*=(float f) {
    m_degree *= f;
    return *this;
  }
  
  Degrees&
  Degrees::operator*=(const Degrees& degree) {
    m_degree *= degree.m_degree;
    return *this;
  }
  
  Degrees
  Degrees::operator/(float f) const {
    return Degrees(m_degree / f);
  }
  
  Degrees&
  Degrees::operator/=(float f) {
    m_degree /= f;
    return *this;
  }
  
  Degrees
  Degrees::operator/(const Degrees& degree) const {
    return Degrees(m_degree / degree.m_degree);
  }
  
  Degrees&
  Degrees::operator/=(const Degrees& degree) {
    m_degree /= degree.m_degree;
    return *this;
  }
  
  bool
  Degrees::operator<(const Degrees& degree) const {
    return m_degree < degree.m_degree;
  }
  
  bool
  Degrees::operator<=(const Degrees& degree) const {
    return m_degree <= degree.m_degree;
  }
  
  bool
  Degrees::operator==(const Degrees& degree) const {
    return m_degree == degree.m_degree;
  }
  
  bool
  Degrees::operator!=(const Degrees& degree) const {
    return m_degree != degree.m_degree;
  }
  
  bool
  Degrees::operator>=(const Degrees& degree) const {
    return m_degree >= degree.m_degree;
  }
  
  bool
  Degrees::operator>(const Degrees& degree) const {
    return m_degree > degree.m_degree;
  }
  
  bool
  Degrees::operator<(const float& f) const {
    f;
    return false;
  }
  
  bool
  Degrees::operator<=(const float& f) const {
    return m_degree < f;
  }
  
  bool
  Degrees::operator==(const float& f) const {
    return m_degree == f;
  }
  
  bool
  Degrees::operator!=(const float& f) const {
    return m_degree != f;
  }
  
  bool
  Degrees::operator>=(const float& f) const {
    return m_degree >= f;
  }
  
  bool
  Degrees::operator>(const float& f) const {
    return m_degree > f;
  }
}