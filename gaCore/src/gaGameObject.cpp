#include "gaGameObject.h"

namespace gaEngineSDK {

  void 
  GameObject::setID(uint32 id) {
    m_id = id;
  }

  uint32
  GameObject::getID() {
    return m_id;
  }

  void 
  GameObject::setLayer(uint32 layer) {
    m_layer = layer;
  }

  uint32
  GameObject::getLayer() {
    return m_layer;
  }

  void
  GameObject::setParent(WeakSPtr<GameObject> parent) {
    m_parent = parent.lock();
  }

  SPtr<GameObject>
  GameObject::getParent() {
    return m_parent;
  }

  void 
  GameObject::setChildrens(Vector<SPtr<GameObject>> children) {
    m_vChildrens = children;
  }

  Vector<SPtr<GameObject>>
  GameObject::getChildrens() {
    return m_vChildrens;
  }
}