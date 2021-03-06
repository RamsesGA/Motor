
#include "gaVector4I.h"

namespace gaEngineSDK {

  Vector4I::Vector4I(int32 x, int32 y, int32 z, int32 w) {
    m_x = x;
    m_y = y;
    m_z = z;
    m_w = w;
  }

  Vector4I::Vector4I(const Vector3I& vector3, int32 w){
    m_x = vector3.m_x;
    m_y = vector3.m_y;
    m_z = vector3.m_z;
    m_w = w;
  }

  /***********************************************************************/
  /**
  * Methods.
  */
  /***********************************************************************/

  void 
  Vector4I::setNewValues(int32 x, int32 y, int32 z, int32 w){
    m_x = x;
    m_y = y;
    m_z = z;
    m_w = w;
  }

  /*************************************************************************/
  /**
  * Mathematical methods.
  */
  /*************************************************************************/

  int32
  Vector4I::magnitude() {
    return Math::sqrti((m_x * m_x) +
                       (m_y * m_y) +
                       (m_z * m_z) +
                       (m_w * m_w));
  }

  int32 
  Vector4I::dotProduct(const Vector4I& vector) const {
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

  bool 
  Vector4I::operator<(const Vector4I& vector) {
    if ((m_x < vector.m_x) &&
        (m_y < vector.m_y) &&
        (m_z < vector.m_z) &&
        (m_w < vector.m_w)) {
      return true;
    }
    return false;
  }

  bool 
  Vector4I::operator<=(const Vector4I& vector) {
    if ((m_x <= vector.m_x) &&
        (m_y <= vector.m_y) &&
        (m_z <= vector.m_z) &&
        (m_w <= vector.m_w)) {
      return true;
    }
    return false;
  }

  bool
  Vector4I::operator>(const Vector4I& vector) {
    if ((m_x > vector.m_x) &&
        (m_y > vector.m_y) &&
        (m_z > vector.m_z) &&
        (m_w > vector.m_w)) {
      return true;
    }
    return false;
  }

  bool 
  Vector4I::operator>=(const Vector4I& vector) {
    if ((m_x >= vector.m_x) &&
        (m_y >= vector.m_y) &&
        (m_z >= vector.m_z) &&
        (m_w >= vector.m_w)) {
      return true;
    }
    return false;
  }

  Vector4I& 
  Vector4I::operator=(const Vector4I& vector) {
    m_x = vector.m_x;
    m_y = vector.m_y;
    m_z = vector.m_z;
    m_w = vector.m_w;
    return *this;
  }

  bool 
  Vector4I::operator==(const Vector4I& vector) {
    if ((m_x == vector.m_x) &&
      (m_y == vector.m_y) &&
      (m_z == vector.m_z) &&
      (m_w == vector.m_w)) {
      return true;
    }
    return false;
  }

  Vector4I 
  Vector4I::operator+(const Vector4I& vector) const {
    return Vector4I(m_x + vector.m_x,
                    m_y + vector.m_y,
                    m_z + vector.m_z,
                    m_w + vector.m_w);
  }

  Vector4I&
  Vector4I::operator+=(const Vector4I& vector) {
    m_x += vector.m_x;
    m_y += vector.m_y;
    m_z += vector.m_z;
    m_w += vector.m_w;
    return *this;
  }

  Vector4I 
  Vector4I::operator-(const Vector4I& vector) const {
    return Vector4I(m_x - vector.m_x,
                    m_y - vector.m_y,
                    m_z - vector.m_z,
                    m_w - vector.m_w);
  }

  Vector4I& 
  Vector4I::operator-=(const Vector4I& vector) {
    m_x -= vector.m_x;
    m_y -= vector.m_y;
    m_z -= vector.m_z;
    m_w -= vector.m_w;
    return *this;
  }

  Vector4I 
  Vector4I::operator*(const int32 vector) const {
    return Vector4I(vector * m_x,
                    vector * m_y,
                    vector * m_z,
                    vector * m_w);
  }

  Vector4I& 
  Vector4I::operator*=(const int32 vector) {
    m_x *= vector;
    m_y *= vector;
    m_z *= vector;
    m_w *= vector;
    return *this;
  }

  Vector4I 
  Vector4I::operator/(const int32 vector) const {
    return Vector4I(m_x / vector,
                    m_y / vector,
                    m_z / vector,
                    m_w / vector);
  }

  Vector4I& 
  Vector4I::operator/=(const int32 vector) {
    m_x = m_x / vector;
    m_y = m_y / vector;
    m_z = m_z / vector;
    m_w = m_w / vector;
    return *this;
  }

  Vector4I 
  Vector4I::operator*(const Vector4I& vector) const {
    Vector4I vectorTemp;

    return Vector4I(vectorTemp.m_x = m_x * vector.m_x,
                    vectorTemp.m_y = m_y * vector.m_y,
                    vectorTemp.m_z = m_z * vector.m_z,
                    vectorTemp.m_w = m_w * vector.m_w);
  }

  int32& 
  Vector4I::operator[](uint32 index){
    return (&m_x)[index];
  }
   
}