
#include "gaVector2I.h"

namespace gaEngineSDK {

  Vector2I::Vector2I(int32 x, int32 y) {
    m_x = x;
    m_y = y;
  }

  /*************************************************************************/
  /**
  * Mathematical normal methods.
  */
  /*************************************************************************/

  int32 Vector2I::magnitude() {
    return Math::sqrti((m_x * m_x) + (m_y * m_y));
  }

  void Vector2I::normalize() {
    int32 magn = magnitude();

    if (magn > 0) {
      int32 oneOverMag = 1 / magn;

      m_x = m_x * oneOverMag;
      m_y = m_y * oneOverMag;
    }

    m_x = 0;
    m_y = 0;
  }

  int32 Vector2I::dotProduct(const Vector2I& vector) const {
    return (m_x * vector.m_x) + (m_y * vector.m_y);
  }

  /*************************************************************************/
  /**
  * Operator overload.
  */
  /*************************************************************************/

  bool Vector2I::operator<(const Vector2I& vector) {
    if (m_x < vector.m_x && m_y < vector.m_y) {
      return true;
    }
    return false;
  }

  bool Vector2I::operator<=(const Vector2I& vector) {
    if (m_x <= vector.m_x && m_y <= vector.m_y) {
      return true;
    }
    return false;
  }

  bool Vector2I::operator>(const Vector2I& vector) {
    if (m_x > vector.m_x && m_y > vector.m_y) {
      return true;
    }
    return false;
  }

  bool Vector2I::operator>=(const Vector2I& vector) {
    if (m_x >= vector.m_x && m_y >= vector.m_y) {
      return true;
    }
    return false;
  }

  Vector2I& Vector2I::operator=(const Vector2I& vector) {
    m_x = vector.m_x;
    m_y = vector.m_y;
    return *this;
  }

  bool Vector2I::operator==(const Vector2I& vector) {
    if (m_x == vector.m_x && m_y == vector.m_y) {
      return true;
    }
    return false;
  }

  Vector2I Vector2I::operator+(const Vector2I& vector) const {
    return Vector2I(m_x + vector.m_x, m_y + vector.m_y);
  }

  Vector2I& Vector2I::operator+=(const Vector2I& vector) {
    m_x += vector.m_x;
    m_y += vector.m_y;
    return *this;
  }

  Vector2I Vector2I::operator-(const Vector2I& vector) const {
    return Vector2I(m_x - vector.m_x, m_y - vector.m_y);
  }

  Vector2I& Vector2I::operator-=(const Vector2I& vector) {
    m_x -= vector.m_x;
    m_y -= vector.m_y;
    return *this;
  }

  Vector2I Vector2I::operator*(const int32 vector) const {
    return Vector2I(vector * m_x, vector * m_y);
  }

  Vector2I& Vector2I::operator*=(const int32 vector) {
    m_x *= vector;
    m_y *= vector;
    return *this;
  }

  Vector2I Vector2I::operator/(const int32 vector) const {
    return Vector2I(m_x / vector, m_y / vector);
  }

  Vector2I& Vector2I::operator/=(const int32 vector) {
    m_x = m_x / vector;
    m_y = m_y / vector;
    return *this;
  }

  Vector2I Vector2I::operator*(const Vector2I& vector) const {
    Vector2I vectorTemp(0,0);

    return Vector2I(vectorTemp.m_x = m_x * vector.m_x,
      vectorTemp.m_y = m_y * vector.m_y);
  }

  /*************************************************************************/
  /**
  * Gets.
  */
  /*************************************************************************/

  int32 Vector2I::getX() {
    return m_x;
  }

  int32 Vector2I::getY() {
    return m_y;
  }
}