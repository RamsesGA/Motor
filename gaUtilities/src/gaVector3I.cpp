
#include "gaVector3I.h"

namespace gaEngineSDK {

  Vector3I::Vector3I(int32 X, int32 Y, int32 Z) {
    x = X;
    y = Y;
    z = Z;
  }

  /***************************************************************************/
  /**
  * Mathematical normal methods.
  */
  /***************************************************************************/

  int32 
  Vector3I::dotProduct(const Vector3I& vector) const {
    return (x * vector.x) + (y * vector.y) + (z * vector.z);
  }

  Vector3I
  Vector3I::crossProduct(const Vector3I& vector) const {
    return Vector3I(y * vector.z - z * vector.y, z * vector.x - x * vector.z, 
                    x * vector.y - y * vector.x);
  }

  /***************************************************************************/
  /**
  * Operator overload.
  */
  /***************************************************************************/

  bool 
  Vector3I::operator<(const Vector3I& vector) {
    if ((x < vector.x) && (y < vector.y) && (z < vector.z)) {
      return true;
    }
    return false;
  }

  bool 
  Vector3I::operator>(const Vector3I& vector) {
    if ((x > vector.x) && (y > vector.y) && (z > vector.z)) {
      return true;
    }
    return false;
  }

  Vector3I& 
  Vector3I::operator=(const Vector3I& vector) {
    x = vector.x;
    y = vector.y;
    z = vector.z;
    return *this;
  }

  bool 
  Vector3I::operator==(const Vector3I& vector) {
    if ((x == vector.x) && (y == vector.y) && (z == vector.z)) {
      return true;
    }
    return false;
  }

  Vector3I 
  Vector3I::operator+(const Vector3I& vector) const {
    return Vector3I(x + vector.x, y + vector.y, z + vector.z);
  }

  Vector3I& 
  Vector3I::operator+=(const Vector3I& vector) {
    x += vector.x;
    y += vector.y;
    z += vector.z;
    return *this;
  }

  Vector3I 
  Vector3I::operator-(const Vector3I& vector) const {
    return Vector3I(x - vector.x, y - vector.y, z - vector.z);
  }

  Vector3I& 
  Vector3I::operator-=(const Vector3I& vector) {
    x -= vector.x;
    y -= vector.y;
    z -= vector.z;
    return *this;
  }

  Vector3I 
  Vector3I::operator*(const int32 vector) const {
    return Vector3I(vector * x, vector * y, vector * z);
  }

  Vector3I&
  Vector3I::operator*=(const int32 vector) {
    x *= vector;
    y *= vector;
    z *= vector;
    return *this;
  }

  Vector3I
  Vector3I::operator*(const Vector3I& vector) const {
    Vector3I vectorTemp(0, 0, 0);

    return Vector3I(vectorTemp.x = x * vector.x, vectorTemp.y = y * vector.y,
                    vectorTemp.z = z * vector.z);
  }
}