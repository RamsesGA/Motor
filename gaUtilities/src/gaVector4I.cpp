#include "gaVector4I.h"

namespace gaEngineSDK {

  Vector4I::Vector4I(int32 X, int32 Y, int32 Z, int32 W) {
    x = X;
    y = Y;
    z = Z;
    w = W;
  }

  Vector4I::Vector4I(const Vector3I& vector3, int32 W){
    x = vector3.x;
    y = vector3.y;
    z = vector3.z;
    w = W;
  }

  /*************************************************************************/
  /**
  * Mathematical methods.
  */
  /*************************************************************************/

  int32 
  Vector4I::dotProduct(const Vector4I& vector) const {
    return (x * vector.x) + (y * vector.y) + (z * vector.z) + (w * vector.w);
  }

  /*************************************************************************/
  /**
  * Operator overload.
  */
  /*************************************************************************/

  bool 
  Vector4I::operator<(const Vector4I& vector) {
    if ((x < vector.x) && (y < vector.y) && (z < vector.z) && (w < vector.w)) {
      return true;
    }
    return false;
  }

  bool
  Vector4I::operator>(const Vector4I& vector) {
    if ((x > vector.x) && (y > vector.y) && (z > vector.z) && (w > vector.w)) {
      return true;
    }
    return false;
  }

  Vector4I& 
  Vector4I::operator=(const Vector4I& vector) {
    x = vector.x;
    y = vector.y;
    z = vector.z;
    w = vector.w;
    return *this;
  }

  bool 
  Vector4I::operator==(const Vector4I& vector) {
    if ((x == vector.x) && (y == vector.y) && (z == vector.z) && (w == vector.w)) {
      return true;
    }
    return false;
  }

  Vector4I 
  Vector4I::operator+(const Vector4I& vector) const {
    return Vector4I(x + vector.x, y + vector.y, z + vector.z, w + vector.w);
  }

  Vector4I&
  Vector4I::operator+=(const Vector4I& vector) {
    x += vector.x;
    y += vector.y;
    z += vector.z;
    w += vector.w;
    return *this;
  }

  Vector4I 
  Vector4I::operator-(const Vector4I& vector) const {
    return Vector4I(x - vector.x,  y - vector.y, z - vector.z, w - vector.w);
  }

  Vector4I& 
  Vector4I::operator-=(const Vector4I& vector) {
    x -= vector.x;
    y -= vector.y;
    z -= vector.z;
    w -= vector.w;
    return *this;
  }

  Vector4I 
  Vector4I::operator*(const int32 vector) const {
    return Vector4I(vector * x, vector * y, vector * z, vector * w);
  }

  Vector4I& 
  Vector4I::operator*=(const int32 vector) {
    x *= vector;
    y *= vector;
    z *= vector;
    w *= vector;
    return *this;
  }

  Vector4I 
  Vector4I::operator*(const Vector4I& vector) const {
    Vector4I vectorTemp;

    return Vector4I(vectorTemp.x = x * vector.x, vectorTemp.y = y * vector.y,
                    vectorTemp.z = z * vector.z, vectorTemp.w = w * vector.w);
  }

  int32& 
  Vector4I::operator[](uint32 index){
    return (&x)[index];
  }
}