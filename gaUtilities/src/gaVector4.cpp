
#include "gaVector4.h"

namespace gaEngineSDK {

  Vector4::Vector4(float x, 
                   float y, 
                   float z, 
                   float w) {
    m_x = x;
    m_y = y;
    m_z = z;
    m_w = w;
  }

  /*Vector4::Vector4(const Vector3& vec3, float w){
    m_x = vec3.m_x;
    m_y = vec3.m_y;
    m_z = vec3.m_z;
    m_w = w;
  }*/

  /*************************************************************************/
  /**
  * Mathematical normal methods.
  */
  /*************************************************************************/

  float
  Vector4::magnitude() {
    return (Math::sqrtf((m_x * m_x) + (m_y * m_y) + (m_z * m_z) + (m_w * m_w)));
  }

  void 
  Vector4::normalize() {
    float magn = magnitude();

    if (magn > 0.0f) {
      float oneOverMag = 1.0f / magn;
      m_x = m_x * oneOverMag;
      m_y = m_y * oneOverMag;
      m_z = m_z * oneOverMag;
      m_w = m_w * oneOverMag;
    }
    else {
      m_x = 0.0f;
      m_y = 0.0f;
      m_z = 0.0f;
      m_w = 0.0f;
    }
  }

  float 
  Vector4::dotProduct(const Vector4& vector) const {
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
  Vector4::operator<(const Vector4& vector) {
    if ((m_x < vector.m_x) &&
        (m_y < vector.m_y) &&
        (m_z < vector.m_z) &&
        (m_w < vector.m_w)) {
      return true;
    }
    return false;
  }

  bool 
  Vector4::operator<=(const Vector4& vector) {
    if ((m_x <= vector.m_x) &&
        (m_y <= vector.m_y) &&
        (m_z <= vector.m_z) &&
        (m_w <= vector.m_w)) {
      return true;
    }
    return false;
  }

  bool
  Vector4::operator>(const Vector4& vector) {
    if ((m_x > vector.m_x) &&
        (m_y > vector.m_y) &&
        (m_z > vector.m_z) &&
        (m_w > vector.m_w)) {
      return true;
    }
    return false;
  }

  bool
  Vector4::operator>=(const Vector4& vector) {
    if ((m_x >= vector.m_x) &&
        (m_y >= vector.m_y) &&
        (m_z >= vector.m_z) &&
        (m_w >= vector.m_w)) {
      return true;
    }
    return false;
  }

  Vector4& 
  Vector4::operator=(const Vector4& vector) {
    m_x = vector.m_x;
    m_y = vector.m_y;
    m_z = vector.m_z;
    m_w = vector.m_w;
    return *this;
  }

  bool 
  Vector4::operator==(const Vector4& vector) {
    if ((m_x == vector.m_x) &&
        (m_y == vector.m_y) &&
        (m_z == vector.m_z) &&
        (m_w == vector.m_w)) {
      return true;
    }
    return false;
  }

  Vector4 
  Vector4::operator+(const Vector4& vector) const {
    return Vector4(m_x + vector.m_x,
                   m_y + vector.m_y,
                   m_z + vector.m_z,
                   m_w + vector.m_w);
  }

  Vector4&
  Vector4::operator+=(const Vector4& vector) {
    m_x += vector.m_x;
    m_y += vector.m_y;
    m_z += vector.m_z;
    m_w += vector.m_w;
    return *this;
  }

  Vector4 
  Vector4::operator-(const Vector4& vector) const {
    return Vector4(m_x - vector.m_x,
                   m_y - vector.m_y,
                   m_z - vector.m_z,
                   m_w - vector.m_w);
  }

  Vector4& 
  Vector4::operator-=(const Vector4& vector) {
    m_x -= vector.m_x;
    m_y -= vector.m_y;
    m_z -= vector.m_z;
    m_w -= vector.m_w;
    return *this;
  }

  Vector4 
  Vector4::operator*(const float vector) const {
    return Vector4(vector * m_x,
                   vector * m_y,
                   vector * m_z,
                   vector * m_w);
  }

  Vector4& 
  Vector4::operator*=(const float vector) {
    m_x *= vector;
    m_y *= vector;
    m_z *= vector;
    m_w *= vector;
    return *this;
  }

  Vector4 
  Vector4::operator/(const float vector) const {
    return Vector4(m_x / vector,
                   m_y / vector,
                   m_z / vector,
                   m_w / vector);
  }

  Vector4& 
  Vector4::operator/=(const float vector) {
    m_x = m_x / vector;
    m_y = m_y / vector;
    m_z = m_z / vector;
    m_w = m_w / vector;
    return *this;
  }

  Vector4 
  Vector4::operator*(const Vector4& vector) const {
    Vector4 vectorTemp(0.0f, 0.0f, 0.0f, 0.0f);

    return Vector4(vectorTemp.m_x = m_x * vector.m_x,
                   vectorTemp.m_y = m_y * vector.m_y,
                   vectorTemp.m_z = m_z * vector.m_z,
                   vectorTemp.m_w = m_w * vector.m_w);
  }
}