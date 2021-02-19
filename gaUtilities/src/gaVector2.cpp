
#include <cmath>

#include <gaVector2.h>

namespace gaEngineSDK {

  Vector2::Vector2(float x, float y) {
    m_x = x;
    m_y = y;
  }

  /*************************************************************************/
  /**
  * Mathematical normal methods.
  */
  /*************************************************************************/

  float Vector2::magnitude() {
    return (std::sqrtf((m_x * m_x) + (m_y * m_y)));
  }

  void Vector2::normalize() {
    float magn = magnitude();

    if (magn > 0.0f) {
      float oneOverMag = 1.0f / magn;
      m_x = m_x * oneOverMag;
      m_y = m_y * oneOverMag;
    }
    m_x = 0.0f;
    m_y = 0.0f;
  }

  float Vector2::dotProduct(const Vector2& vector) const {
    return (m_x * vector.m_x) + (m_y * vector.m_y);
  }

  /*************************************************************************/
  /**
  * Operator overload.
  */
  /*************************************************************************/

  bool Vector2::operator<(const Vector2& vector) {
    if (m_x < vector.m_x && m_y < vector.m_y) {
      return true;
    }
    return false;
  }

  bool Vector2::operator<=(const Vector2& vector) {
    if (m_x <= vector.m_x && m_y <= vector.m_y) {
      return true;
    }
    return false;
  }

  bool Vector2::operator>(const Vector2& vector) {
    if (m_x > vector.m_x && m_y > vector.m_y) {
      return true;
    }
    return false;
  }

  bool Vector2::operator>=(const Vector2& vector) {
    if (m_x >= vector.m_x && m_y >= vector.m_y) {
      return true;
    }
    return false;
  }

  Vector2& Vector2::operator=(const Vector2& vector) {
    m_x = vector.m_x;
    m_y = vector.m_y;
    return *this;
  }

  bool Vector2::operator==(const Vector2& vector) {
    if (m_x == vector.m_x && m_y == vector.m_y) {
      return true;
    }
    return false;
  }

  Vector2 Vector2::operator+(const Vector2& vector) const {
    return Vector2(m_x + vector.m_x, m_y + vector.m_y);
  }

  Vector2& Vector2::operator+=(const Vector2& vector) {
    m_x += vector.m_x;
    m_y += vector.m_y;
    return *this;
  }

  Vector2 Vector2::operator-(const Vector2& vector) const {
    return Vector2(m_x - vector.m_x, m_y - vector.m_y);
  }

  Vector2& Vector2::operator-=(const Vector2& vector) {
    m_x -= vector.m_x;
    m_y -= vector.m_y;
    return *this;
  }

  Vector2 Vector2::operator*(const float vector) const {
    return Vector2(vector * m_x, vector * m_y);
  }

  Vector2& Vector2::operator*=(const float vector) {
    m_x *= vector;
    m_y *= vector;
    return *this;
  }

  Vector2 Vector2::operator/(const float vector) const {
    return Vector2(m_x / vector, m_y / vector);
  }

  Vector2& Vector2::operator/=(const float vector) {
    m_x = m_x / vector;
    m_y = m_y / vector;
    return *this;
  }

  Vector2 Vector2::operator*(const Vector2& vector) const {
    Vector2 vectorTemp;

    return Vector2(vectorTemp.m_x = m_x * vector.m_x,
      vectorTemp.m_y = m_y * vector.m_y);
  }
}