
#include "gaDegrees.h"

namespace gaEngineSDK {

  Degrees::Degrees(float degree){
      m_degree = degree;
  }
  
  float 
  Degrees::degreesToRadians(float radian) {
    return (radian * (m_pi / 180));
  }
}