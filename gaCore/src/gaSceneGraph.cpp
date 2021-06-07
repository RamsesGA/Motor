#include "gaSceneGraph.h"

namespace gaEngineSDK
{
  void 
  SceneGraph::onInit(String nameRoot) {

  }

  void 
  SceneGraph::onUpdate(const float& deltaTime) {

  }

  SPtr<GameObject> 
  SceneGraph::createNewGameObj() {
    GameObject* gameObj = new GameObject();
    gameObj->setLayer();
    return nullptr;
  }
}