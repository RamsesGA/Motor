
#include "gaVector2I.h"

namespace gaEngineSDK {

  Vector2I::Vector2I(int32 X, int32 Y) {
    x = X;
    y = Y;
  }

  /***************************************************************************/
  /**
  * Mathematical normal methods.
  */
  /***************************************************************************/

  int32 
  Vector2I::dotProduct(const Vector2I& vector) const {
    return (x * vector.x) + (y * vector.y);
  }

  /***************************************************************************/
  /**
  * Operator overload.
  */
  /***************************************************************************/

  bool 
  Vector2I::operator<(const Vector2I& vector) {
    if (x < vector.x && y < vector.y) {
      return true;
    }
    return false;
  }

  bool 
  Vector2I::operator>(const Vector2I& vector) {
    if (x > vector.x && y > vector.y) {
      return true;
    }
    return false;
  }

  Vector2I& 
  Vector2I::operator=(const Vector2I& vector) {
    x = vector.x;
    y = vector.y;
    return *this;
  }

  bool 
  Vector2I::operator==(const Vector2I& vector) {
    if (x == vector.x && y == vector.y) {
      return true;
    }
    return false;
  }

  Vector2I 
  Vector2I::operator+(const Vector2I& vector) const {
    return Vector2I(x + vector.x, y + vector.y);
  }

  Vector2I& 
  Vector2I::operator+=(const Vector2I& vector) {
    x += vector.x;
    y += vector.y;
    return *this;
  }

  Vector2I
  Vector2I::operator-(const Vector2I& vector) const {
    return Vector2I(x - vector.x, y - vector.y);
  }

  Vector2I& 
  Vector2I::operator-=(const Vector2I& vector) {
    x -= vector.x;
    y -= vector.y;
    return *this;
  }

  Vector2I 
  Vector2I::operator*(const int32 vector) const {
    return Vector2I(vector * x, vector * y);
  }

  Vector2I& 
  Vector2I::operator*=(const int32 vector) {
    x *= vector;
    y *= vector;
    return *this;
  }

  Vector2I 
  Vector2I::operator*(const Vector2I& vector) const {
    Vector2I vectorTemp(0,0);

    return 
    Vector2I(vectorTemp.x = x * vector.x, vectorTemp.y = y * vector.y);
  }
}