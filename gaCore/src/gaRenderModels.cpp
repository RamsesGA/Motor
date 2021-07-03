#include "gaRenderModels.h"
#include "gaMesh.h"
#include "gaGraphicsApi.h"
#include "gaResourceManager.h"

namespace gaEngineSDK {
  void 
  RenderModels::update(ResourceManager& resource, const float& deltaTime) {
    if (nullptr == m_pResourceMang) {
      m_pResourceMang.reset(&resource);
    }

    if (nullptr == m_pResourceMang) {
      m_timeOfAnimation = 0.0f;
      return;
    }
      
    if (nullptr != m_currentAnimation) {
      if (m_playAnimation) {
        m_timeOfAnimation += deltaTime;
      }

      if (m_timeOfAnimation >= m_currentAnimation->m_animDuration) {
        m_timeOfAnimation = 0;
      }

      uint32 meshNum = 0;

      for (auto mesh : m_pResourceMang->getMeshes()) {
        mesh->m_cbBonesTransform = &m_meshBones[meshNum];
        mesh->animated(resource ,m_timeOfAnimation, m_currentAnimation);
        ++meshNum;
      }
    }
  }

  void 
  RenderModels::drawModel(WeakSPtr<ResourceManager> resource, WeakSPtr<ConstantBuffer> cbBone) {
    if (nullptr == m_pResourceMang) {
      m_pResourceMang.reset(resource.lock().get());
    }

    uint32 meshNum = 0;
    auto myGraphicApi = g_graphicApi().instancePtr();

    for (auto mesh : m_pResourceMang->getMeshes()) {
      myGraphicApi->updateConstantBuffer(&m_meshBones[meshNum], cbBone);
      
      myGraphicApi->setVertexBuffer(mesh->m_pVertexBuffer);
      myGraphicApi->setIndexBuffer(mesh->m_pIndexBuffer);

      myGraphicApi->drawIndex(mesh->getNumIndices(), 0, 0);
      ++meshNum;
     }
  }

  void 
  RenderModels::setMeshBones(ResourceManager& resource) {
    m_meshBones.clear();
    m_meshBones.resize(resource.getMeshes().size());
  }
}