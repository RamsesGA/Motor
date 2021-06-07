#include "gaSceneNode.h"

namespace gaEngineSDK {
  void
  SceneNode::onInit(String nodeName, WeakSPtr<SceneNode> nodeParent) {
    m_nodeName = nodeName;
    m_pNodeParent = nodeParent;
  }

}