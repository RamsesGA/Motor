
#include <cmath>
#include <gaMath.h>

/*************************************************************************/
/**
* Trigonometric methods
*/
/*************************************************************************/

template<typename type>
inline type Math::cos(type data){
  return std::cos(data);
}

template<typename type>
inline type Math::sin(type data) {
  return std::sin(data);
}

template<typename type>
inline type Math::tan(type data) {
  return std::tan(data);
}

template<typename type>
inline type Math::arcCos(type data) {
  return std::acos(data);
}

template<typename type>
inline type Math::arcSin(type data) {
  return std::asin(data);
}

template<typename type>
inline type Math::arcTan(type data) {
  return std::atan(data);
}

/*************************************************************************/
/**
* Hyperbolic methods
*/
/*************************************************************************/

template<typename type>
inline type Math::cosHB(type data) {
  return std::cosh(data);
}

template<typename type>
inline type Math::sinHB(type data) {
  return std::sinh(data);
}

template<typename type>
inline type Math::tanHB(type data) {
  return std::tanh(data);
}

template<typename type>
inline type Math::arcCosHB(type data) {
  return std::acosh(data);
}

template<typename type>
inline type Math::arcSinHB(type data) {
  return std::asinh(data);
}

template<typename type>
inline type Math::arcTanHB(type data) {
  return std::atanh(data);
}


/*************************************************************************/
/**
* Exponential and logarithmic functions
*/
/*************************************************************************/

template<typename type>
type Math::exp(type data){
  return std::exp(data);
}

template<typename type>
type Math::log(type data) {
  return std::log(data);
}

/*************************************************************************/
/**
* Power functions
*/
/*************************************************************************/

template<typename type>
type Math::pow(type data, type exponent) {
  return std::pow(data, exponent);
}

template<typename type>
type Math::sqrt(type data) {
  return std::sqrt(data);
}

template<typename type>
type Math::hypo(type x, type y) {
  return std::hypot(x, y);
}