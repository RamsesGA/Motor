
#include "gaVector4.h"

namespace gaEngineSDK {
Vector4::Vector4(float X, float Y, float Z, float W) {
    x = X; y = Y; z = Z; w = W;
  }
  
  /***************************************************************************/
  /**
  * Mathematical normal methods.
  */
  /***************************************************************************/
  
  float
  Vector4::magnitude() {
    return (Math::sqrtf((x * x) + (y * y) + (z * z) + (w * w)));
  }
  
  void
  Vector4::normalize() {
    float magn = magnitude();
  
    if (magn > 0.0f) {
      float oneOverMag = 1.0f / magn;
      x = x * oneOverMag; y = y * oneOverMag; z = z * oneOverMag; w = w * oneOverMag;
    }
    else {
      x = 0.0f; y = 0.0f; z = 0.0f; w = 0.0f;
    }
  }
  
  float
  Vector4::dotProduct(const Vector4& vector) const {
    return (x * vector.x) + (y * vector.y) + (z * vector.z) + (w * vector.w);
  }
  
  /***************************************************************************/
  /**
  * Operator overload.
  */
  /***************************************************************************/
  
  bool
  Vector4::operator<(const Vector4& vector) const {
    if ((x < vector.x) && (y < vector.y) && (z < vector.z) && (w < vector.w)) {
      return true;
    }
    return false;
  }
  
  bool
  Vector4::operator>(const Vector4& vector) const {
    if ((x > vector.x) && (y > vector.y) && (z > vector.z) && (w > vector.w)) {
      return true;
    }
    return false;
  }
  
  Vector4&
  Vector4::operator=(const Vector4& vector) {
    x = vector.x; y = vector.y; z = vector.z; w = vector.w;
    return *this;
  }
  
  bool
  Vector4::operator==(const Vector4& vector) const {
    if ((x == vector.x) && (y == vector.y) && (z == vector.z) && (w == vector.w)) {
      return true;
    }
    return false;
  }
  
  Vector4
  Vector4::operator+(const Vector4& vector) const {
    return Vector4(x + vector.x, y + vector.y, z + vector.z, w + vector.w);
  }
  
  Vector4&
  Vector4::operator+=(const Vector4& vector) {
    x += vector.x; y += vector.y; z += vector.z; w += vector.w;
    return *this;
  }
  
  Vector4
  Vector4::operator-(const Vector4& vector) const {
    return Vector4(x - vector.x, y - vector.y, z - vector.z, w - vector.w);
  }
  
  Vector4&
  Vector4::operator-=(const Vector4& vector) {
    x -= vector.x; y -= vector.y; z -= vector.z; w -= vector.w;
    return *this;
  }
  
  Vector4
  Vector4::operator*(const float vector) const {
    return Vector4(vector * x, vector * y, vector * z, vector * w);
  }
  
  Vector4&
  Vector4::operator*=(const float vector) {
    x *= vector; y *= vector; z *= vector; w *= vector;
    return *this;
  }
  
  Vector4
  Vector4::operator*(const Vector4& vector) const {
    Vector4 vectorTemp(0.0f, 0.0f, 0.0f, 0.0f);
  
    return Vector4(vectorTemp.x = x * vector.x, 
                   vectorTemp.y = y * vector.y,
                   vectorTemp.z = z * vector.z, 
                   vectorTemp.w = w * vector.w);
  }
}