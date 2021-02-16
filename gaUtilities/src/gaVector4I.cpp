
#include <cmath>
#include <gaVector4I.h>

namespace gaEngineSDK {

  Vector4I::Vector4I(int32 x, int32 y, int32 z, int32 w) {
    m_x = x;
    m_y = y;
    m_z = z;
    m_w = w;
  }

  /*************************************************************************/
  /**
  * Mathematical normal methods.
  */
  /*************************************************************************/

  int32 Vector4I::magnitude() {
    return static_cast<int32>(std::sqrtf((m_x * m_x) +
                                         (m_y * m_y) +
                                         (m_z * m_z) +
                                         (m_w * m_w)));
  }

  void Vector4I::normalize() {
    int32 magn = magnitude();

    if (magn > 0) {
      int32 oneOverMag = 1 / magn;
      m_x = m_x * oneOverMag;
      m_y = m_y * oneOverMag;
      m_z = m_z * oneOverMag;
      m_w = m_w * oneOverMag;
    }
    m_x = 0;
    m_y = 0;
    m_z = 0;
    m_w = 0;
  }

  int32 Vector4I::dotProduct(const Vector4I& vector) const {
    return (m_x * vector.m_x) +
      (m_y * vector.m_y) +
      (m_z * vector.m_z) +
      (m_w * vector.m_w);
  }

  /*************************************************************************/
  /**
  * Operator overload.
  */
  /*************************************************************************/

  bool Vector4I::operator<(const Vector4I& vector) {
    if ((m_x < vector.m_x) &&
      (m_y < vector.m_y) &&
      (m_z < vector.m_z) &&
      (m_w < vector.m_w)) {
      return true;
    }
    return false;
  }

  bool Vector4I::operator<=(const Vector4I& vector) {
    if ((m_x <= vector.m_x) &&
      (m_y <= vector.m_y) &&
      (m_z <= vector.m_z) &&
      (m_w <= vector.m_w)) {
      return true;
    }
    return false;
  }

  bool Vector4I::operator>(const Vector4I& vector) {
    if ((m_x > vector.m_x) &&
      (m_y > vector.m_y) &&
      (m_z > vector.m_z) &&
      (m_w > vector.m_w)) {
      return true;
    }
    return false;
  }

  bool Vector4I::operator>=(const Vector4I& vector) {
    if ((m_x >= vector.m_x) &&
      (m_y >= vector.m_y) &&
      (m_z >= vector.m_z) &&
      (m_w >= vector.m_w)) {
      return true;
    }
    return false;
  }

  Vector4I& Vector4I::operator=(const Vector4I& vector) {
    m_x = vector.m_x;
    m_y = vector.m_y;
    m_z = vector.m_z;
    m_w = vector.m_w;
    return *this;
  }

  bool Vector4I::operator==(const Vector4I& vector) {
    if ((m_x == vector.m_x) &&
      (m_y == vector.m_y) &&
      (m_z == vector.m_z) &&
      (m_w == vector.m_w)) {
      return true;
    }
    return false;
  }

  Vector4I Vector4I::operator+(const Vector4I& vector) const {
    return Vector4I(m_x + vector.m_x,
      m_y + vector.m_y,
      m_z + vector.m_z,
      m_w + vector.m_w);
  }

  Vector4I& Vector4I::operator+=(const Vector4I& vector) {
    m_x += vector.m_x;
    m_y += vector.m_y;
    m_z += vector.m_z;
    m_w += vector.m_w;
    return *this;
  }

  Vector4I Vector4I::operator-(const Vector4I& vector) const {
    return Vector4I(m_x - vector.m_x,
      m_y - vector.m_y,
      m_z - vector.m_z,
      m_w - vector.m_w);
  }

  Vector4I& Vector4I::operator-=(const Vector4I& vector) {
    m_x -= vector.m_x;
    m_y -= vector.m_y;
    m_z -= vector.m_z;
    m_w -= vector.m_w;
    return *this;
  }

  Vector4I Vector4I::operator*(const int32 vector) const {
    return Vector4I(vector * m_x,
      vector * m_y,
      vector * m_z,
      vector * m_w);
  }

  Vector4I& Vector4I::operator*=(const int32 vector) {
    m_x *= vector;
    m_y *= vector;
    m_z *= vector;
    m_w *= vector;
    return *this;
  }

  Vector4I Vector4I::operator/(const int32 vector) const {
    return Vector4I(m_x / vector,
      m_y / vector,
      m_z / vector,
      m_w / vector);
  }

  Vector4I& Vector4I::operator/=(const int32 vector) {
    m_x = m_x / vector;
    m_y = m_y / vector;
    m_z = m_z / vector;
    m_w = m_w / vector;
    return *this;
  }

  Vector4I Vector4I::operator*(const Vector4I& vector) const {
    Vector4I vectorTemp;

    return Vector4I(vectorTemp.m_x = m_x * vector.m_x,
      vectorTemp.m_y = m_y * vector.m_y,
      vectorTemp.m_z = m_z * vector.m_z,
      vectorTemp.m_w = m_w * vector.m_w);
  }

  /*************************************************************************/
  /**
  * Sets.
  */
  /*************************************************************************/

  void Vector4I::setX(int32 x) {
    m_x = x;
  }

  void Vector4I::setY(int32 y) {
    m_y = y;
  }

  void Vector4I::setZ(int32 z) {
    m_z = z;
  }

  void Vector4I::setW(int32 w) {
    m_w = w;
  }

  /*************************************************************************/
  /**
  * Gets.
  */
  /*************************************************************************/

  int32 Vector4I::getX() {
    return m_x;
  }

  int32 Vector4I::getY() {
    return m_y;
  }

  int32 Vector4I::getZ() {
    return m_z;
  }

  int32 Vector4I::getW() {
    return m_w;
  }
}