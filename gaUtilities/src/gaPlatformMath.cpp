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
  PlatformMath::taylorSin(const float& x) {
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
  PlatformMath::taylorCos(const float& x) {
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

  float 
  PlatformMath::abs(float val) {
    return ::fabsf(val);
  }

  float 
  PlatformMath::invSqrt(float val) {
    return 1.0f / std::sqrtf(val);
  }

  float
  PlatformMath::cos(float data) {
    return std::cos(data);
  }

  float 
  PlatformMath::sin(float data) {
    return std::sin(data);
  }

  float 
  PlatformMath::tan(float data) {
    return std::tan(data);
  }

  float 
  PlatformMath::arcCos(float data) {
    return std::acos(data);
  }

  float 
  PlatformMath::arcSin(float data) {
    return std::asin(data);
  }

  float 
  PlatformMath::arcTan(float data) {
    return std::atan(data);
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
  PlatformMath::cosHB(float data) {
    return std::cosh(data);
  }

  float 
  PlatformMath::sinHB(float data) {
    return std::sinh(data);
  }

  float 
  PlatformMath::tanHB(float data) {
    return std::tanh(data);
  }

  float 
  PlatformMath::arcCosHB(float data) {
    return std::acosh(data);
  }

  float 
  PlatformMath::arcSinHB(float data) {
    return std::asinh(data);
  }

  float 
  PlatformMath::arcTanHB(float data) {
    return std::atanh(data);
  }


  /*************************************************************************/
  /**
  * Exponential and logarithmic functions
  */
  /*************************************************************************/

  float 
  PlatformMath::exp(float data) {
    return std::exp(data);
  }

  float 
  PlatformMath::log(float data) {
    return std::log(data);
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
  PlatformMath::sqrtf(float data) {
    return std::sqrtf(data);
  }

  float
  PlatformMath::hypo(float x, float y) {
    return std::hypot(x, y);
  }
}