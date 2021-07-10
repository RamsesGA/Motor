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

        if (0 != m_pResource->getMeshes()[meshNum]->m_textures.size()) {

          //TODO: necesito corregir el sampler :C

          myGraphicApi->setShaderResourceView(
            m_pResource->getMeshes()[meshNum]->m_textures, meshNum, 1);
        }

        myGraphicApi->setVertexBuffer(mesh->m_pVertexBuffer);
        myGraphicApi->setIndexBuffer(mesh->m_pIndexBuffer);

        myGraphicApi->drawIndex(mesh->getNumIndices(), 0, 0);
        ++meshNum;
      }
    }
  }

  void
  RenderModels::changeModel(uint32 numMesh) {
    m_meshBones.clear();
    m_meshBones.resize(numMesh);
  }

  void
  RenderModels::setModel(WeakSPtr<Model> model) {
    m_model = model.lock();

    changeModel((uint32)m_model->getMeshesInfo().size());

    m_currentAnimation = nullptr;
    m_animationNumber = 0;
    m_timeOfAnimation = 0.0f;
  }
}