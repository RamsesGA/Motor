#include "gaActor.h"
#include "gaRenderModels.h"
#include "gaCamera.h"
#include "gaTransform.h"
#include "gaSceneNode.h"

namespace gaEngineSDK {
  Actor::Actor(String actorName) {
    m_actorName = actorName;
  }

  void 
  Actor::removeComponent(TYPE_COMPONENTS::E typeComponent) {}

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

  SPtr<Component> 
  Actor::getComponent(TYPE_COMPONENTS::E typeComponent) {
    return SPtr<Component>();
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
  Actor::setComponent(TYPE_COMPONENTS::E typeComponent, WeakSPtr<Component> compoInfo) {
    m_vComponents.push_back(typeComponent);
    m_component = compoInfo.lock();
  }

}