
#include <cmath>

#include <gaVector3.h>

Vector3::Vector3(float x, float y, float z) {
  m_x = x;
  m_y = y;
  m_z = z;
}

/*************************************************************************/
/**
* Mathematical normal methods.
*/
/*************************************************************************/

float Vector3::magnitude() {
  return (std::sqrtf((m_x * m_x) + (m_y * m_y) + (m_z * m_z)));
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

Vector3 Vector3::crossProduct(const Vector3& vector) const{
  return Vector3();
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
  Vector3 vectorTemp;

  return Vector3(vectorTemp.m_x = m_x * vector.m_x,
                 vectorTemp.m_y = m_y * vector.m_y,
                 vectorTemp.m_z = m_z * vector.m_z);
}

/*************************************************************************/
/**
* Gets.
*/
/*************************************************************************/

float Vector3::getX() {
  return m_x;
}

float Vector3::getY() {
  return m_y;
}

float Vector3::getZ(){
  return m_z;
}
