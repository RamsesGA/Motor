
#include "gaDegrees.h"

namespace gaEngineSDK {

  template<typename type>
  inline type& Degrees::degreesToRadians(type radian) {
    return (radian * (m_pi / 180));
  }
}