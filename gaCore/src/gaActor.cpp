#include "gaActor.h"
#include "gaRenderModels.h"
#include "gaCamera.h"
#include "gaTransform.h"
#include "gaSceneNode.h"

namespace gaEngineSDK {
  void 
  Actor::update(const float& deltaTime) {
    for (auto component : m_mapComponents) {
      component.second.get()->update(deltaTime);
    }
  }

  void
  Actor::render() {
    auto render = m_mapComponents.find(TYPE_COMPONENTS::kRenderModel);

    if (render != m_mapComponents.end()) {
      auto model = reinterpret_cast<RenderModels*>(render->second.get());
      model->drawModel();
    }
  }

  void 
  Actor::removeComponent(TYPE_COMPONENTS::E typeComponent) {
    auto comp = m_mapComponents.find(typeComponent);

    if (comp != m_mapComponents.end()) {
      m_mapComponents.erase(typeComponent);
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

  SPtr<Component> 
  Actor::getComponent(TYPE_COMPONENTS::E typeComponent) {
    auto comp = m_mapComponents.find(typeComponent);

    if (comp != m_mapComponents.end()) {
      return comp->second;
    }
    return nullptr;
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

  SPtr<Component> 
  Actor::setComponent(TYPE_COMPONENTS::E typeComponent, SPtr<Component> component) {
    auto comp = m_mapComponents.find(typeComponent);

    //Component alredy exist in this object.
    if (comp != m_mapComponents.end()) {
      return nullptr;
    }

    //We save the component and the gameobject.
    if (nullptr != component) {
      component->setActor(this);
      m_mapComponents.insert(std::pair <TYPE_COMPONENTS::E,
        SPtr<Component>>(typeComponent, component));
      return component;
    }

    Component* addComponent = nullptr;
    switch (typeComponent) {

    case gaEngineSDK::TYPE_COMPONENTS::kRenderModel:
      addComponent = new RenderModels();
      break;

    case gaEngineSDK::TYPE_COMPONENTS::kTransform:
      addComponent = new Transform();
      break;

    case gaEngineSDK::TYPE_COMPONENTS::kCamera:
      addComponent = new Camera();
      break;

    case gaEngineSDK::TYPE_COMPONENTS::kTexture:
      break;

    default:
      return nullptr;
    }

    //If the component don´t exists.
    if (nullptr == addComponent) {
      return nullptr;
    }

    addComponent->setActor(this);

    m_mapComponents.insert(std::pair<TYPE_COMPONENTS::E,
      SPtr<Component>>(typeComponent, addComponent));

    return SPtr<Component>(addComponent);
  }

}