
#include <cmath>

#include <gaVector3I.h>

Vector3I::Vector3I(int32 x, int32 y, int32 z) {
  m_x = x;
  m_y = y;
  m_z = z;
}

/*************************************************************************/
/**
* Mathematical normal methods.
*/
/*************************************************************************/

int32 Vector3I::magnitude() {
  return static_cast<int32>(std::sqrt((m_x * m_x) + 
                                      (m_y * m_y) + 
                                      (m_z * m_z)));
}

void Vector3I::normalize() {
  int32 magn = magnitude();

  if (magn > 0) {
    int32 oneOverMag = 1 / magn;
    m_x = m_x * oneOverMag;
    m_y = m_y * oneOverMag;
    m_z = m_z * oneOverMag;
  }
  m_x = 0;
  m_y = 0;
  m_z = 0;
}

int32 Vector3I::dotProduct(const Vector3I& vector) const {
  return (m_x * vector.m_x) + (m_y * vector.m_y) + (m_z * vector.m_z);
}

Vector3I Vector3I::crossProduct(const Vector3I& vector) const {
  return Vector3I(m_y * vector.m_z - m_z * vector.m_y,
                  m_z * vector.m_x - m_x * vector.m_z,
                  m_x * vector.m_y - m_y * vector.m_x);
}

/*************************************************************************/
/**
* Operator overload.
*/
/*************************************************************************/

bool Vector3I::operator<(const Vector3I& vector) {
  if ((m_x < vector.m_x) &&
      (m_y < vector.m_y) &&
      (m_z < vector.m_z)) {
    return true;
  }
  return false;
}

bool Vector3I::operator<=(const Vector3I& vector) {
  if ((m_x <= vector.m_x) &&
      (m_y <= vector.m_y) &&
      (m_z <= vector.m_z)) {
    return true;
  }
  return false;
}

bool Vector3I::operator>(const Vector3I& vector) {
  if ((m_x > vector.m_x) &&
      (m_y > vector.m_y) &&
      (m_z > vector.m_z)) {
    return true;
  }
  return false;
}

bool Vector3I::operator>=(const Vector3I& vector) {
  if ((m_x >= vector.m_x) &&
      (m_y >= vector.m_y) &&
      (m_z >= vector.m_z)) {
    return true;
  }
  return false;
}

Vector3I& Vector3I::operator=(const Vector3I& vector) {
  m_x = vector.m_x;
  m_y = vector.m_y;
  m_z = vector.m_z;
  return *this;
}

bool Vector3I::operator==(const Vector3I& vector) {
  if ((m_x == vector.m_x) &&
      (m_y == vector.m_y) &&
      (m_z == vector.m_z)) {
    return true;
  }
  return false;
}

Vector3I Vector3I::operator+(const Vector3I& vector) const {
  return Vector3I(m_x + vector.m_x,
                  m_y + vector.m_y,
                  m_z + vector.m_z);
}

Vector3I& Vector3I::operator+=(const Vector3I& vector) {
  m_x += vector.m_x;
  m_y += vector.m_y;
  m_z += vector.m_z;
  return *this;
}

Vector3I Vector3I::operator-(const Vector3I& vector) const {
  return Vector3I(m_x - vector.m_x,
                  m_y - vector.m_y,
                  m_z - vector.m_z);
}

Vector3I& Vector3I::operator-=(const Vector3I& vector) {
  m_x -= vector.m_x;
  m_y -= vector.m_y;
  m_z -= vector.m_z;
  return *this;
}

Vector3I Vector3I::operator*(const int32 vector) const {
  return Vector3I(vector * m_x,
                  vector * m_y,
                  vector * m_z);
}

Vector3I& Vector3I::operator*=(const int32 vector) {
  m_x *= vector;
  m_y *= vector;
  m_z *= vector;
  return *this;
}

Vector3I Vector3I::operator/(const int32 vector) const {
  return Vector3I(m_x / vector,
                  m_y / vector,
                  m_z / vector);
}

Vector3I& Vector3I::operator/=(const int32 vector) {
  m_x = m_x / vector;
  m_y = m_y / vector;
  m_z = m_z / vector;
  return *this;
}

Vector3I Vector3I::operator*(const Vector3I& vector) const {
  Vector3I vectorTemp;

  return Vector3I(vectorTemp.m_x = m_x * vector.m_x,
                  vectorTemp.m_y = m_y * vector.m_y,
                  vectorTemp.m_z = m_z * vector.m_z);
}

/*************************************************************************/
/**
* Sets.
*/
/*************************************************************************/

void Vector3I::setX(int32 x) {
  m_x = x;
}

void Vector3I::setY(int32 y) {
  m_y = y;
}

void Vector3I::setZ(int32 z) {
  m_z = z;
}

/*************************************************************************/
/**
* Gets.
*/
/*************************************************************************/

int32 Vector3I::getX() {
  return m_x;
}

int32 Vector3I::getY() {
  return m_y;
}

int32 Vector3I::getZ() {
  return m_z;
}