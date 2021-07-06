#include "gaComponent.h"

namespace gaEngineSDK {
  TYPE_COMPONENTS::E 
  Component::getTypeComponents() const {
    return m_typeComponents;
  }

  void
  Component::setActor(Actor* newObj) {
    m_newActorObj = newObj;
  }
}