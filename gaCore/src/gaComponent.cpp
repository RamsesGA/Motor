#include "gaComponent.h"
#include "gaResourceManager.h"

namespace gaEngineSDK {
  Component::Component(WeakSPtr<ResourceManager> modelInfo) {
    m_modelInfo = modelInfo.lock();
    m_typeComponent = TYPE_COMPONENTS::kRenderModel;
  }
}