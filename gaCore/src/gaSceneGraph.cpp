#include "gaSceneGraph.h"
#include "gaTransform.h"
#include "gaActor.h"
#include "gaModels.h"

namespace gaEngineSDK {
  SceneGraph::SceneGraph() { 
    m_root.reset(new SceneNode());

    SPtr<Actor> newActor = make_shared<Actor>();

    newActor->init("Root Actor");

    m_root->setActorNode(newActor);
  }

  void
  SceneGraph::update(const float& deltaTime) {
    m_root->update(deltaTime);
  }

  void 
  SceneGraph::render() {
    m_root->render();
  }

  SPtr<SceneNode>
  SceneGraph::createNewActor(WeakSPtr<Actor> actor, WeakSPtr<SceneNode> parent) {
    //auto localParent = parent.lock();
    //
    //if (nullptr == localParent.get()) {
    //  return m_root->createNewActorNode(actor, m_root);
    //}
    //
    //return localParent->createNewActorNode(actor, localParent);

    auto localParent = parent.lock();
    if (nullptr == localParent) {
      return m_root->createNewActorNode(actor, m_root);
    }

    return m_root->createNewActorNode(actor, localParent);
  }

  void 
  SceneGraph::clearSceneGraph() {
    m_root->clearSceneNodes();
    
    m_root->getChildNodes().clear();

    /*
    * generar una luz para mover
    */
  }

  SceneGraph&
  g_sceneGraph() {
    return SceneGraph::instance();
  }
}