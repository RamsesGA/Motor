#include "gaSceneGraph.h"

namespace gaEngineSDK
{
  void 
  SceneGraph::onInit() {
    GameObject* root = new GameObject();

  }

  void 
  SceneGraph::onUpdate(const float& deltaTime) {
  }

  void 
  SceneGraph::onDelete() {
  }

  SPtr<GameObject> 
  SceneGraph::createNewGameObj() {
    return SPtr<GameObject>();
  }
}