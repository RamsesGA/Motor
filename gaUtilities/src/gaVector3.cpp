#include "gaVector3.h"

namespace gaEngineSDK {
  Vector3::Vector3(float X, float Y, float Z) {
    x = X; y = Y; z = Z;
  }

  /***************************************************************************/
  /**
  * Mathematical methods.
  */
  /***************************************************************************/

  float
  Vector3::magnitude() {
    return Math::sqrtf((x * x) + (y * y) + (z * z));
  }

  void
  Vector3::normalize() {
    float magn = magnitude();

    if (magn > 0.0f) {
      float oneOverMag = 1.0f / magn;
      x = x * oneOverMag; y = y * oneOverMag; z = z * oneOverMag;
    }
    else {
      x = 0.0f; y = 0.0f; z = 0.0f;
    }
  }

  float
  Vector3::dotProduct(const Vector3& vector) const {
    return (x * vector.x) + (y * vector.y) + (z * vector.z);
  }

  Vector3
  Vector3::crossProduct(const Vector3& vector) const {
    return Vector3(y * vector.z - z * vector.y, 
                   z * vector.x - x * vector.z,
                   x * vector.y - y * vector.x);
  }

  /***************************************************************************/
  /**
  * Operator overload.
  */
  /***************************************************************************/

  bool
  Vector3::operator<(const Vector3& vector) {
    if ((x < vector.x) && (y < vector.y) && (z < vector.z)) {
      return true;
    }
    return false;
  }

  bool
  Vector3::operator>(const Vector3& vector) {
    if ((x > vector.x) && (y > vector.y) && (z > vector.z)) {
      return true;
    }
    return false;
  }

  Vector3&
  Vector3::operator=(const Vector3& vector) {
    x = vector.x; y = vector.y; z = vector.z;
    return *this;
  }

  bool
  Vector3::operator==(const Vector3& vector) {
    if ((x == vector.x) && (y == vector.y) && (z == vector.z)) {
      return true;
    }
    return false;
  }

  Vector3
  Vector3::operator+(const Vector3& vector) const {
    return Vector3(x + vector.x, y + vector.y, z + vector.z);
  }

  Vector3&
  Vector3::operator+=(const Vector3& vector) {
    x += vector.x; y += vector.y; z += vector.z;
    return *this;
  }

  Vector3
  Vector3::operator-(const Vector3& vector) const {
    return Vector3(x - vector.x, y - vector.y, z - vector.z);
  }

  Vector3&
  Vector3::operator-=(const Vector3& vector) {
    x -= vector.x; y -= vector.y; z -= vector.z;
    return *this;
  }

  Vector3
  Vector3::operator*(const float vector) const {
    return Vector3(vector * x, vector * y, vector * z);
  }

  Vector3&
  Vector3::operator*=(const float vector) {
    x *= vector; y *= vector; z *= vector;
    return *this;
  }

  Vector3
  Vector3::operator*(const Vector3& vector) const {
    Vector3 vectorTemp(0.0f, 0.0f, 0.0f);

    return Vector3(vectorTemp.x = x * vector.x, 
                   vectorTemp.y = y * vector.y,
                   vectorTemp.z = z * vector.z);
  }

  float&
  Vector3::operator[](uint32 index) {
    return (&x)[index];
  }

  bool 
  Vector3::operator!=(const Vector3& vector) {
    if ((x != vector.x) ||
        (y != vector.y)||
        (z != vector.z)) {
      return true;
    }

    return false;
  }

}