
#include "gaVector3.h"

namespace gaEngineSDK {

  Vector3::Vector3(float x, float y, float z) {
    m_x = x;
    m_y = y;
    m_z = z;
  }

  Vector3::Vector3(const Vector4 & vec4){
    m_x = vec4.m_x;
    m_y = vec4.m_y;
    m_z = vec4.m_z;
  }

  /***********************************************************************/
  /**
  * Methods.
  */
  /***********************************************************************/

  void Vector3::setNewValues(float x, float y, float z){
    m_x = x;
    m_y = y;
    m_z = z;
  }

  /*************************************************************************/
  /**
  * Mathematical methods.
  */
  /*************************************************************************/

  float Vector3::magnitude() {
    return Math::sqrtf((m_x * m_x) + (m_y * m_y) + (m_z * m_z));
  }

  void Vector3::normalize() {
    float magn = magnitude();

    if (magn > 0.0f) {
      float oneOverMag = 1.0f / magn;
      m_x = m_x * oneOverMag;
      m_y = m_y * oneOverMag;
      m_z = m_z * oneOverMag;
    }
    m_x = 0.0f;
    m_y = 0.0f;
    m_z = 0.0f;
  }

  float Vector3::dotProduct(const Vector3& vector) const {
    return (m_x * vector.m_x) + (m_y * vector.m_y) + (m_z * vector.m_z);
  }

  Vector3 Vector3::crossProduct(const Vector3& vector) const {
    return Vector3(m_y * vector.m_z - m_z * vector.m_y,
      m_z * vector.m_x - m_x * vector.m_z,
      m_x * vector.m_y - m_y * vector.m_x);
  }

  /*************************************************************************/
  /**
  * Operator overload.
  */
  /*************************************************************************/

  bool Vector3::operator<(const Vector3& vector) {
    if ((m_x < vector.m_x) &&
      (m_y < vector.m_y) &&
      (m_z < vector.m_z)) {
      return true;
    }
    return false;
  }

  bool Vector3::operator<=(const Vector3& vector) {
    if ((m_x <= vector.m_x) &&
      (m_y <= vector.m_y) &&
      (m_z <= vector.m_z)) {
      return true;
    }
    return false;
  }

  bool Vector3::operator>(const Vector3& vector) {
    if ((m_x > vector.m_x) &&
      (m_y > vector.m_y) &&
      (m_z > vector.m_z)) {
      return true;
    }
    return false;
  }

  bool Vector3::operator>=(const Vector3& vector) {
    if ((m_x >= vector.m_x) &&
      (m_y >= vector.m_y) &&
      (m_z >= vector.m_z)) {
      return true;
    }
    return false;
  }

  Vector3& Vector3::operator=(const Vector3& vector) {
    m_x = vector.m_x;
    m_y = vector.m_y;
    m_z = vector.m_z;
    return *this;
  }

  bool Vector3::operator==(const Vector3& vector) {
    if ((m_x == vector.m_x) &&
      (m_y == vector.m_y) &&
      (m_z == vector.m_z)) {
      return true;
    }
    return false;
  }

  Vector3 Vector3::operator+(const Vector3& vector) const {
    return Vector3(m_x + vector.m_x,
      m_y + vector.m_y,
      m_z + vector.m_z);
  }

  Vector3& Vector3::operator+=(const Vector3& vector) {
    m_x += vector.m_x;
    m_y += vector.m_y;
    m_z += vector.m_z;
    return *this;
  }

  Vector3 Vector3::operator-(const Vector3& vector) const {
    return Vector3(m_x - vector.m_x,
      m_y - vector.m_y,
      m_z - vector.m_z);
  }

  Vector3& Vector3::operator-=(const Vector3& vector) {
    m_x -= vector.m_x;
    m_y -= vector.m_y;
    m_z -= vector.m_z;
    return *this;
  }

  Vector3 Vector3::operator*(const float vector) const {
    return Vector3(vector * m_x,
      vector * m_y,
      vector * m_z);
  }

  Vector3& Vector3::operator*=(const float vector) {
    m_x *= vector;
    m_y *= vector;
    m_z *= vector;
    return *this;
  }

  Vector3 Vector3::operator/(const float vector) const {
    return Vector3(m_x / vector,
      m_y / vector,
      m_z / vector);
  }

  Vector3& Vector3::operator/=(const float vector) {
    m_x = m_x / vector;
    m_y = m_y / vector;
    m_z = m_z / vector;
    return *this;
  }

  Vector3 Vector3::operator*(const Vector3& vector) const {
    Vector3 vectorTemp(0.0f, 0.0f, 0.0f);

    return Vector3(vectorTemp.m_x = m_x * vector.m_x,
                   vectorTemp.m_y = m_y * vector.m_y,
                   vectorTemp.m_z = m_z * vector.m_z);
  }

  Vector3 Vector3::operator^(const Vector3& vector) const {
    return Vector3(m_y * vector.m_z - m_z * vector.m_y,
                   m_z * vector.m_x - m_x * vector.m_z,
                   m_x * vector.m_y - m_y * vector.m_x);
  }

  float& Vector3::operator[](uint32 index){
    return (&m_x)[index];
  }

}