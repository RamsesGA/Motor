
#include "gaRadians.h"

namespace gaEngineSDK {
  Radians::Radians(float radians){
    m_radian = radians;
  }

  float Radians::radiansToDegrees(float radian) {
    return (radian * (180 / m_pi));
  }
}