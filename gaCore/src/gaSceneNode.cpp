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
    //SPtr<SceneNode> newNode;
    //SPtr<SceneNode> localParent(this);
    //
    //newNode.reset(new SceneNode(actor, localParent));
    //
    //m_pChildNodes.push_back(newNode);
    //
    //return newNode;

    SPtr<SceneNode> newNode;
    newNode.reset(new SceneNode(actor, parent));

    m_pChildNodes.push_back(newNode);

    return newNode;
  }

  void 
  SceneNode::update(const float& deltaTime) {
    m_pActor->actorUpdate(deltaTime);

    if (m_pChildNodes.empty()) {
      return;
    }

    for (auto accessNode : m_pChildNodes) {
      accessNode->update(deltaTime);
    }
  }

  void 
  SceneNode::render() {
    m_pActor->actorRender();

    if (m_pChildNodes.empty()) {
      return;
    }

    for (auto accessNode : m_pChildNodes) {
      accessNode->render();
    }
  }

  void 
  SceneNode::clearSceneNodes() {
    uint32 sizeChilds = m_pChildNodes.size();
    for (uint32 i = 0; i < sizeChilds; ++i) {
      m_pChildNodes[i]->clearSceneNodes();

      m_pChildNodes[i].reset();
    }
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

  SPtr<Actor>&
  SceneNode::getActorNode() {
    return m_pActor;
  }

  WeakSPtr<SceneNode>
  SceneNode::getParentNode() {
    return m_pParentNode;
  }

  Vector<SPtr<SceneNode>>&
  SceneNode::getChildNodes() {
    return m_pChildNodes;
  }
}