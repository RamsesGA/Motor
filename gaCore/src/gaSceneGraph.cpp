#include "gaSceneGraph.h"
#include "gaTransform.h"
#include "gaActor.h"

namespace gaEngineSDK {
  SceneGraph::SceneGraph() { 
    m_root.reset(new SceneNode());

    SPtr<Actor> newActor;
    newActor.reset(new Actor("Root actor"));
    newActor->setComponent(TYPE_COMPONENTS::kTransform);

    m_root->setActorNode(newActor);
  }

  SPtr<SceneNode>
  SceneGraph::createNewActor(WeakSPtr<Actor> actor, WeakSPtr<SceneNode> parent) {
    if (nullptr == parent.lock().get()) {
      m_root->createNewActorNode(actor, m_root);
      return m_root;
    }

    m_root->createNewActorNode(actor, parent);
    return m_root;
  }

  SceneGraph& 
  g_sceneGraph() {
    return SceneGraph::instance();
  }
}