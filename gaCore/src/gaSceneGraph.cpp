#include "gaSceneGraph.h"
#include "gaTransform.h"
#include "gaActor.h"

namespace gaEngineSDK {
  SceneGraph::SceneGraph() { 
    m_root.reset(new SceneNode());

    SPtr<Actor> newActor;
    newActor.reset(new Actor());
    newActor->setIsSelected(false);
    newActor->setActorName("Root actor");
    newActor->setComponent(TYPE_COMPONENTS::kTransform);

    m_root->setActorNode(newActor);
    m_root->setParentNode(SPtr<SceneNode>(nullptr));
    m_root->setChildNode(SPtr<SceneNode>(nullptr));
  }

  void
  SceneGraph::createNewActor(WeakSPtr<Actor> actor, WeakSPtr<SceneNode> parent) {
    if ((nullptr != actor.lock().get()) && (nullptr != parent.lock().get()) && 
        (nullptr != m_root)) {
      m_root->createNewActorNode(actor, parent);
    }

  }
}