
#include <gaRadians.h>

template<typename type>
inline type Radians::radiansToDegrees(type radian){
  return (radian * (180 / m_pi));
}