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

  /*************************************************************************/
  /**
  * Trigonometric methods
  */
  /*************************************************************************/

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