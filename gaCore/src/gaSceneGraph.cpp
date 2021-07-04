#include "gaSceneGraph.h"
#include "gaTransform.h"

namespace gaEngineSDK
{
  void 
  SceneGraph::init(String nameRoot) {
    Actor* tempRootActor = new Actor();

    tempRootActor->setLayer();
    tempRootActor->setIsSelected(true);
    tempRootActor->setComponent(TYPE_COMPONENTS::kTransform);
  }

  void 
  SceneGraph::update(const float& deltaTime) {

  }

  SPtr<GameObject> 
  SceneGraph::createNewGameObj() {
    GameObject* gameObj = new GameObject();
    gameObj->setLayer();
    return nullptr;
  }
}