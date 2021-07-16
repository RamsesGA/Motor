#include "gaActor.h"
#include "gaCamera.h"
#include "gaTransform.h"
#include "gaSceneNode.h"

namespace gaEngineSDK {
  Actor::Actor(String actorName) {
    m_actorName = actorName;
    m_actorTransform.reset(new Transform());
  }

  void 
  Actor::removeComponent() {}

  void 
  Actor::actorUpdate(const float& deltaTime) {
    for (auto accessComponent : m_vComponents) {
      accessComponent->update(deltaTime);
    }
  }

  void
  Actor::actorRender() {
    for (auto accessComponent : m_vComponents) {
      accessComponent->render();
    }
  }

  /***************************************************************************/
  /**
  * Gets.
  */
  /***************************************************************************/

  bool 
  Actor::getIsSelected() {
    return m_isSelected;
  }

  String
  Actor::getActorName() {
    return m_actorName;
  }

  /***************************************************************************/
  /**
  * Sets.
  */
  /***************************************************************************/

  void 
  Actor::setIsSelected(bool isSelect) {
    m_isSelected = isSelect;
  }

  void 
  Actor::setActorName(String name) {
    m_actorName = name;
  }

  void
  Actor::setComponent(WeakSPtr<Component> compoInfo) {
    m_vComponents.push_back(compoInfo.lock());
  }
}