#include "gaSceneGraph.h"
#include "gaTransform.h"
#include "gaActor.h"
#include "gaModels.h"

namespace gaEngineSDK {
  SceneGraph::SceneGraph() { 
    m_root.reset(new SceneNode());

    SPtr<Actor> newActor;
    newActor.reset(new Actor("Root actor"));

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

  void
  SceneGraph::createNewActor(WeakSPtr<Actor> actor, WeakSPtr<SceneNode> parent) {
    if (nullptr == parent.lock().get()) {
      m_root->createNewActorNode(actor, m_root);
      return;
    }

    m_root->createNewActorNode(actor, parent);
  }

  SceneGraph& 
  g_sceneGraph() {
    return SceneGraph::instance();
  }
}