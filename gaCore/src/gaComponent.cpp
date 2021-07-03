#include "gaComponent.h"

namespace gaEngineSDK {
  void
  Component::update(const float& deltatime) { }

  TYPE_COMPONENTS::E 
  Component::getTypeComponents() const {
    return m_typeComponents;
  }

  void
  Component::setGameObject(GameObject* newObj) {
    m_newObject = newObj;
  }
}