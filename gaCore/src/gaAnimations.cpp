#include "gaAnimations.h"

namespace gaEngineSDK {
  void 
  Animations::setAnimName(String animName) {
    m_vAnimationsNames.push_back(animName);
  }

  void 
  Animations::setNumAnims(uint32 numAnims) {
    m_numAnimations = numAnims;
  }
}