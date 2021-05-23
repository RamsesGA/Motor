#include "gaSceneGraph.h"

namespace gaEngineSDK
{
  void 
  SceneGraph::onInit() {
    GameObject* root = new GameObject();
    root->setLayer();
  }

  void 
  SceneGraph::onUpdate(const float& deltaTime) {
  }

  void 
  SceneGraph::onDelete() {
  }

  SPtr<GameObject> 
  SceneGraph::createNewGameObj() {
    GameObject* gameObj = new GameObject();
    gameObj->setLayer();
    return nullptr;
  }
}