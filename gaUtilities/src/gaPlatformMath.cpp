#include "gaPlatformMath.h"

namespace gaEngineSDK {

  /*************************************************************************/
  /**
  * Constants variables
  */
  /*************************************************************************/

  const float PlatformMath::PI = 3.14159265f;
  const float PlatformMath::EULER_NUMBER = 2.718f;
  const float PlatformMath::SMALL_NUMBER = 1.e-6f;
  const float PlatformMath::KINDA_SMALL_NUMBER = 1.e-4f;
  const float PlatformMath::BIG_NUMBER = 3.4e+38f;
  const float PlatformMath::DELTA = 1.e-5f;
  const float PlatformMath::FOV = 0.78539816339f;
  const float PlatformMath::DEG2RAD = static_cast<float>((4.0f * std::atan(1.0f)) / 180.0f);
  const float PlatformMath::RAD2DEG = static_cast<float>(180.0f / (4.0f * std::atan(1.0f)));
  const float PlatformMath::EPSILONF = FLT_EPSILON;
  const float PlatformMath::TWO_PI = (2.0f * (4.0f * std::atan(1.0f)));

  /*************************************************************************/
  /**
  * Trigonometric methods
  */
  /*************************************************************************/

  float 
  PlatformMath::unwindDegrees(float A) {
    while (180.f < A) {
      A -= 360.f;
    }

    while (-180.f > A) {
      A += 360.f;
    }

    return A;
  }

  float 
  PlatformMath::unwindRadians(float A) {
    while (PI < A) {
      A -= TWO_PI;
    }

    while (-PI > A) {
      A += TWO_PI;
    }

    return A;
  }

  float 
  PlatformMath::fastSin(const float& x) {
    static uint32 fac3 = 6;
    static uint32 fac5 = 120;
    static uint32 fac7 = 5040;
    static uint32 fac9 = 362880;
    static uint32 fac11 = 39916800;

    float sum = x;
    sum -= pow(x, 3) / fac3;
    sum += pow(x, 5) / fac5;
    sum -= pow(x, 7) / fac7;
    sum += pow(x, 9) / fac9;
    sum -= pow(x, 11) / fac11;
    return sum;
  }

  float 
  PlatformMath::fastCos(const float& x) {
    float sum = 1;
    static uint32 fac2 = 2;
    static uint32 fac4 = 24;
    static uint32 fac6 = 720;
    static uint32 fac8 = 40320;
    static uint32 fac10 = 3628800;
    static uint32 fac12 = 479001600;

    sum -= pow(x, 2) / fac2;
    sum += pow(x, 4) / fac4;
    sum -= pow(x, 6) / fac6;
    sum += pow(x, 8) / fac8;
    sum -= pow(x, 10) / fac10;
    sum -= pow(x, 12) / fac12;
    return sum;
  }

  template<typename T>
  static T PlatformMath::abs(T val) {
    return ::abs(val);
  }

  float 
  PlatformMath::abs(float val) {
    return ::fabsf(val);
  }

  float 
  PlatformMath::invSqrt(float val) {
    return 1.0f / std::sqrtf(val);
  }

  template<typename T>
  static T PlatformMath::cos(T x) {
    return std::cos(x);
  }

  float
  PlatformMath::cos(float x) {
    return std::cos(x);
  }

  template<typename T>
  static T PlatformMath::sin(T x) {
    return std::sin(x);
  }

  float 
  PlatformMath::sin(float x) {
    return std::sin(x);
  }

  template<typename T>
  static T PlatformMath::tan(T x) {
    return std::tan(x);
  }

  float 
  PlatformMath::tan(float x) {
    return std::tan(x);
  }

  float 
  PlatformMath::arcCos(float x) {
    return std::acos(x);
  }

  float 
  PlatformMath::arcSin(float x) {
    return std::asin(x);
  }

  float 
  PlatformMath::arcTan(float x) {
    return std::atan(x);
  }

  float 
  PlatformMath::arcTan(float val, float val2) {
    return std::atan2f(val, val2);
  }

  /*************************************************************************/
  /**
  * Hyperbolic methods
  */
  /*************************************************************************/

  float 
  PlatformMath::cosHB(float x) {
    return std::cosh(x);
  }

  float 
  PlatformMath::sinHB(float x) {
    return std::sinh(x);
  }

  float 
  PlatformMath::tanHB(float x) {
    return std::tanh(x);
  }

  float 
  PlatformMath::arcCosHB(float x) {
    return std::acosh(x);
  }

  float 
  PlatformMath::arcSinHB(float x) {
    return std::asinh(x);
  }

  float 
  PlatformMath::arcTanHB(float x) {
    return std::atanh(x);
  }


  /*************************************************************************/
  /**
  * Exponential and logarithmic functions
  */
  /*************************************************************************/

  float 
  PlatformMath::exp(float x) {
    return std::exp(x);
  }

  float 
  PlatformMath::log(float x) {
    return std::log(x);
  }

  /*************************************************************************/
  /**
  * Power functions
  */
  /*************************************************************************/

  float 
  PlatformMath::pow(float data, float exponent) {
    return std::pow(data, exponent);
  }

  float 
  PlatformMath::sqrtf(float x) {
    return std::sqrtf(x);
  }

  float
  PlatformMath::hypo(float x, float y) {
    return std::hypot(x, y);
  }
}