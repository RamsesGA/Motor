
#include "gaVector2.h"

namespace gaEngineSDK {

  Vector2::Vector2(float X, float Y) {
    x = X;
    y = Y;
  }

  /*************************************************************************/
  /**
  * Mathematical normal methods.
  */
  /*************************************************************************/

  float
  Vector2::magnitude() {
    float temp = (x * x) + (y * y);
    temp = Math::sqrtf(temp);
    return (temp);
  }

  void 
  Vector2::normalize() {
    float magn = magnitude();

    if (magn > 0.0f) {
      float oneOverMag = 1.0f / magn;
      x = x * oneOverMag;
      y = y * oneOverMag;
    }
    else {
      x = 0.0f;
      y = 0.0f;
    }
  }

  float 
  Vector2::dotProduct(const Vector2& vector) const {
    return (x * vector.x) + (y * vector.y);
  }

  /*************************************************************************/
  /**
  * Operator overload.
  */
  /*************************************************************************/

  bool
  Vector2::operator<(const Vector2& vector) {
    if (x < vector.x && y < vector.y) {
      return true;
    }
    return false;
  }

  bool
  Vector2::operator>(const Vector2& vector) {
    if (x > vector.x && y > vector.y) {
      return true;
    }
    return false;
  }

  Vector2& 
  Vector2::operator=(const Vector2& vector) {
    x = vector.x;
    y = vector.y;
    return *this;
  }

  bool 
  Vector2::operator==(const Vector2& vector) {
    if (x == vector.x && y == vector.y) {
      return true;
    }
    return false;
  }

  Vector2 
  Vector2::operator+(const Vector2& vector) const {
    return Vector2(x + vector.x, y + vector.y);
  }

  Vector2& 
  Vector2::operator+=(const Vector2& vector) {
    x += vector.x;
    y += vector.y;
    return *this;
  }

  Vector2 
  Vector2::operator-(const Vector2& vector) const {
    return Vector2(x - vector.x, y - vector.y);
  }

  Vector2& 
  Vector2::operator-=(const Vector2& vector) {
    x -= vector.x;
    y -= vector.y;
    return *this;
  }

  Vector2 
  Vector2::operator*(const float vector) const {
    return Vector2(vector * x, vector * y);
  }

  Vector2& 
  Vector2::operator*=(const float vector) {
    x *= vector;
    y *= vector;
    return *this;
  }

  Vector2
  Vector2::operator*(const Vector2& vector) const {
    Vector2 vectorTemp(0.0f, 0.0f);

    return Vector2(vectorTemp.x = x * vector.x, vectorTemp.y = y * vector.y);
  }
}