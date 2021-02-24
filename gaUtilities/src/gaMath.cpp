#include "gaMath.h"

namespace gaEngineSDK {

  /*************************************************************************/
  /**
  * Constants variables
  */
  /*************************************************************************/

  const float Math::PI = 3.14159265f;

  /*************************************************************************/
  /**
  * Trigonometric methods
  */
  /*************************************************************************/

  float Math::cos(float data){
    return std::cos(data);
  }

  float Math::sin(float data) {
    return std::sin(data);
  }

  float Math::tan(float data) {
    return std::tan(data);
  }

  float Math::arcCos(float data) {
    return std::acos(data);
  }

  float Math::arcSin(float data) {
    return std::asin(data);
  }

  float Math::arcTan(float data) {
    return std::atan(data);
  }

  /*************************************************************************/
  /**
  * Hyperbolic methods
  */
  /*************************************************************************/

  float Math::cosHB(float data) {
    return std::cosh(data);
  }

  float Math::sinHB(float data) {
    return std::sinh(data);
  }

  float Math::tanHB(float data) {
    return std::tanh(data);
  }

  float Math::arcCosHB(float data) {
    return std::acosh(data);
  }

  float Math::arcSinHB(float data) {
    return std::asinh(data);
  }

  float Math::arcTanHB(float data) {
    return std::atanh(data);
  }


  /*************************************************************************/
  /**
  * Exponential and logarithmic functions
  */
  /*************************************************************************/

  float Math::exp(float data) {
    return std::exp(data);
  }

  float Math::log(float data) {
    return std::log(data);
  }

  /*************************************************************************/
  /**
  * Power functions
  */
  /*************************************************************************/

  float Math::pow(float data, float exponent) {
    return std::pow(data, exponent);
  }

  float Math::sqrtf(float data) {
    return std::sqrtf(data);
  }

  int32 Math::sqrti(int32 data){
    return static_cast<int32>(std::sqrt(data) + 0.5f);
  }

  float Math::hypo(float x, float y) {
    return std::hypot(x, y);
  }
  
}