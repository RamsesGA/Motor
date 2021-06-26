#include "gaComponent.h"

namespace gaEngineSDK {
  TYPE_COMPONENTS::E 
  Component::getTypeComponents() const {
    return m_typeComponents;
  }

  void
  Component::update(const float& deltatime) { }

  void
  Component::setGameObject(GameObject* newObj) {
    m_newObject = newObj;
  }
}