#include "gaGameObject.h"
#include "gaCamera.h"

namespace gaEngineSDK {
  void 
  GameObject::update(const float& deltaTime) { }
  
  void 
  GameObject::draw() { }

  SPtr<Component> 
  GameObject::addComponent(TYPE_COMPONENTS::E typeComponent, SPtr<Component> component) {
    auto comp = m_mapComponents.find(typeComponent);

    //Component alredy exist in this object.
    if (comp != m_mapComponents.end()) {
      return nullptr;
    }

    if (nullptr != component) {
      component->setGameObject(this);
      m_mapComponents.insert(std::pair <TYPE_COMPONENTS::E, 
                             SPtr<Component>>(typeComponent, component));
      return component;
    }

    //TODO: falta complementar
    Component* addComponent = nullptr;
    switch (typeComponent) {

      case gaEngineSDK::TYPE_COMPONENTS::TRANSFORM:
        break;

      case gaEngineSDK::TYPE_COMPONENTS::RENDERMODEL:
        break;

      case gaEngineSDK::TYPE_COMPONENTS::TEXTURE:
        break;

      case gaEngineSDK::TYPE_COMPONENTS::CAMERA:
        break;

      case gaEngineSDK::TYPE_COMPONENTS::KNUMCOMPONENTS:
        break;

      default:
        return nullptr;
    }

    //If the component don´t exists.
    if (nullptr == addComponent) {
      return nullptr;
    }

    addComponent->setGameObject(this);

    m_mapComponents.insert(std::pair<TYPE_COMPONENTS::E, 
                           SPtr<Component>>(typeComponent, addComponent));

    return SPtr<Component>(addComponent);
  }

  void 
  GameObject::removeComponent(TYPE_COMPONENTS::E typeComponent) {
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

  uint32
  GameObject::getID() {
    return m_id;
  }

  uint32
  GameObject::getLayer() {
    return m_layer;
  }

  SPtr<GameObject>
  GameObject::getParent() {
    return m_parent;
  }

  Vector<SPtr<GameObject>>
  GameObject::getChildrens() {
    return m_vChildrens;
  }

  bool
  GameObject::getIsSelected() {
    return m_isSelected;
  }

  SPtr<Component> 
  GameObject::getComponent(TYPE_COMPONENTS::E typeComponent) {
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
  GameObject::setID(uint32 id) {
    m_id = id;
  }

  void
  GameObject::setLayer(uint32 layer) {
    m_layer = layer;
  }

  void
  GameObject::setParent(WeakSPtr<GameObject> parentObject) {
    m_parent = parentObject.lock();
  }

  void 
  GameObject::setChildrens(Vector<SPtr<GameObject>> children) {
    m_vChildrens = children;
  }

  void 
  GameObject::setIsSelected(bool isSelect) {
    m_isSelected = isSelect;
  }
}