
#include "gaDegrees.h"
#include "gaMath.h"

namespace gaEngineSDK {

  Degrees::Degrees(float degree){
      m_degree = degree;
  }
  
  float 
  Degrees::degreesToRadians(float radian) {
    return (radian * (Math::PI / 180));
  }
}