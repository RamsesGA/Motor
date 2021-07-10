#include "gaSceneNode.h"

namespace gaEngineSDK {
  SceneNode::SceneNode(WeakSPtr<Actor> actor, WeakSPtr<SceneNode> parent) {
    m_pActor = actor.lock();
    m_pParentNode = parent.lock();
  }

  void 
  SceneNode::detectSelectedNode() {
    m_pSelectedNode = nullptr;

    for (auto nodes : m_pChildNodes) {
      if (nodes->m_pActor->getIsSelected()) {
        m_pSelectedNode = nodes;
      }
      nodes->detectSelectedNode();
    }
  }

  SPtr<SceneNode>
    SceneNode::createNewActorNode(WeakSPtr<Actor> actor, WeakSPtr<SceneNode> parent) {
    SPtr<SceneNode> newNode;
    newNode.reset(new SceneNode(actor, parent));

    m_pActor->setActorName("Actor##");
    m_pActor->setComponent(TYPE_COMPONENTS::kTransform);

    m_pChildNodes.push_back(newNode);

    return newNode;
  }

  /***************************************************************************/
  /**
  * Sets.
  */
  /***************************************************************************/

  void 
  SceneNode::setActorNode(WeakSPtr<Actor> actor) {
    m_pActor = actor.lock();
  }

  void
  SceneNode::setParentNode(WeakSPtr<SceneNode> parent) {
    m_pParentNode = parent.lock();
  }

  void
  SceneNode::setChildNode(WeakSPtr<SceneNode> child) {
    m_pChildNodes.push_back(child.lock());
  }

  /***************************************************************************/
  /**
  * Gets.
  */
  /***************************************************************************/

  SPtr<Actor>
  SceneNode::getActorNode() {
    return m_pActor;
  }

  WeakSPtr<SceneNode>
  SceneNode::getParentNode() {
    return m_pParentNode;
  }

  Vector<SPtr<SceneNode>>
  SceneNode::getChildNodes() {
    return m_pChildNodes;
  }
  
}