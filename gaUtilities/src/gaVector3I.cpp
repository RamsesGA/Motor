
#include "gaVector3I.h"

namespace gaEngineSDK {

  Vector3I::Vector3I(int32 x, int32 y, int32 z) {
    m_x = x;
    m_y = y;
    m_z = z;
  }

  /***************************************************************************/
  /**
  * Mathematical normal methods.
  */
  /***************************************************************************/

  int32 
  Vector3I::dotProduct(const Vector3I& vector) const {
    return (m_x * vector.m_x) + (m_y * vector.m_y) + (m_z * vector.m_z);
  }

  Vector3I
  Vector3I::crossProduct(const Vector3I& vector) const {
    return Vector3I(m_y * vector.m_z - m_z * vector.m_y,
                    m_z * vector.m_x - m_x * vector.m_z,
                    m_x * vector.m_y - m_y * vector.m_x);
  }

  /***************************************************************************/
  /**
  * Operator overload.
  */
  /***************************************************************************/

  bool 
  Vector3I::operator<(const Vector3I& vector) {
    if ((m_x < vector.m_x) &&
        (m_y < vector.m_y) &&
        (m_z < vector.m_z)) {
      return true;
    }
    return false;
  }

  bool 
  Vector3I::operator>(const Vector3I& vector) {
    if ((m_x > vector.m_x) &&
        (m_y > vector.m_y) &&
        (m_z > vector.m_z)) {
      return true;
    }
    return false;
  }

  Vector3I& 
  Vector3I::operator=(const Vector3I& vector) {
    m_x = vector.m_x;
    m_y = vector.m_y;
    m_z = vector.m_z;
    return *this;
  }

  bool 
  Vector3I::operator==(const Vector3I& vector) {
    if ((m_x == vector.m_x) &&
        (m_y == vector.m_y) &&
        (m_z == vector.m_z)) {
      return true;
    }
    return false;
  }

  Vector3I 
  Vector3I::operator+(const Vector3I& vector) const {
    return Vector3I(m_x + vector.m_x,
                    m_y + vector.m_y,
                    m_z + vector.m_z);
  }

  Vector3I& 
  Vector3I::operator+=(const Vector3I& vector) {
    m_x += vector.m_x;
    m_y += vector.m_y;
    m_z += vector.m_z;
    return *this;
  }

  Vector3I 
  Vector3I::operator-(const Vector3I& vector) const {
    return Vector3I(m_x - vector.m_x,
                    m_y - vector.m_y,
                    m_z - vector.m_z);
  }

  Vector3I& 
  Vector3I::operator-=(const Vector3I& vector) {
    m_x -= vector.m_x;
    m_y -= vector.m_y;
    m_z -= vector.m_z;
    return *this;
  }

  Vector3I 
  Vector3I::operator*(const int32 vector) const {
    return Vector3I(vector * m_x,
                    vector * m_y,
                    vector * m_z);
  }

  Vector3I&
  Vector3I::operator*=(const int32 vector) {
    m_x *= vector;
    m_y *= vector;
    m_z *= vector;
    return *this;
  }

  Vector3I
  Vector3I::operator*(const Vector3I& vector) const {
    Vector3I vectorTemp(0, 0, 0);

    return Vector3I(vectorTemp.m_x = m_x * vector.m_x,
                    vectorTemp.m_y = m_y * vector.m_y,
                    vectorTemp.m_z = m_z * vector.m_z);
  }
}