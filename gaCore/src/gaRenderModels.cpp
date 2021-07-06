#include "gaRenderModels.h"
#include "gaMesh.h"
#include "gaGraphicsApi.h"
#include "gaResourceManager.h"

namespace gaEngineSDK {
  void 
  RenderModels::update(ResourceManager& resource, const float& deltaTime) {
    if (nullptr == m_pResource) {
      m_pResource.reset(&resource);
    }
    
    if (nullptr == m_pResource) {
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

      for (auto mesh : m_pResource->getMeshes()) {
        mesh->m_cbBonesTrans = &m_meshBones[meshNum];
        mesh->animated(m_timeOfAnimation, m_currentAnimation, m_pResource);
        ++meshNum;
      }
    }
  }

  void 
  RenderModels::drawModel() {
    if (nullptr != m_pResource) {
      auto myGraphicApi = g_graphicApi().instancePtr();

      uint32 meshNum = 0;

      for (auto mesh : m_pResource->getMeshes()) {
        myGraphicApi->updateConstantBuffer(&m_meshBones[meshNum], 
                                           myGraphicApi->getConstBufferBones());

        myGraphicApi->setVertexBuffer(mesh->m_pVertexBuffer);
        myGraphicApi->setIndexBuffer(mesh->m_pIndexBuffer);

        myGraphicApi->drawIndex(mesh->getNumIndices(), 0, 0);
        ++meshNum;
      }
    }
  }

  void 
  RenderModels::setMeshBones(ResourceManager& resource) {
    m_meshBones.clear();
    m_meshBones.resize(resource.getMeshes().size());
  }
}