#include "gaRenderModels.h"
#include "gaMesh.h"
#include "gaGraphicsApi.h"
#include "gaResourceManager.h"

namespace gaEngineSDK {
  void 
  RenderModels::init(WeakSPtr<Model> model) {
    m_pModel = model.lock();
    changeModel((uint32)m_pModel->m_vMeshes.size());
  }

  void 
  RenderModels::onUpdate(const float& deltaTime) {
    if (nullptr == m_pModel) {
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

      for (auto mesh : m_pModel.get()->m_vMeshes) {
        mesh->m_cbBonesTransform = &m_meshBones[meshNum];
        mesh->animated(m_timeOfAnimation, m_currentAnimation);
        ++meshNum;
      }
    }
  }

  void 
  RenderModels::drawModel(ResourceManager& resource, WeakSPtr<ConstantBuffer> cbBone) {
    if ((m_pModel == nullptr) ||
        (nullptr == &cbBone)) {
      return;
    }

    uint32 meshNum = 0;
    auto myGraphicApi = g_graphicApi().instancePtr();

    for (auto mesh : m_pModel.get()->m_vMeshes) {
      myGraphicApi->updateConstantBuffer(&m_meshBones[meshNum], *cbBone.lock().get());

      auto tempAlgo = resource.getSamplerInfo();

      myGraphicApi->setSamplerState(0, tempAlgo, resource.m_newMesh->m_textures[meshNum].texture);
      myGraphicApi->setShaderResourceView(resource.m_newMesh->m_textures[meshNum].texture, meshNum, 1);

      myGraphicApi->setVertexBuffer(*mesh.get()->m_pVertexBuffer);
      myGraphicApi->setIndexBuffer(*mesh.get()->m_pIndexBuffer);

      myGraphicApi->drawIndex(mesh.get()->getNumIndices(), 0, 0);
      ++meshNum;
    }
  }

  void
  RenderModels::changeModel(uint32 numMeshNewMode) {
    m_meshBones.clear();
    m_meshBones.resize(numMeshNewMode);
  }

  void 
  RenderModels::noneAnimation() {
    m_meshBones.clear();
    if (nullptr != m_pModel) {
      m_meshBones.resize(m_pModel->m_vMeshes.size());
    }
  }
}