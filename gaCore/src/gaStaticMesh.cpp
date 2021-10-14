#include "gaStaticMesh.h"
#include "gaGraphicsApi.h"
#include "gaModels.h"
#include "gaMesh.h"
#include "gaActor.h"
#include "gaTransform.h"

using std::make_shared;

namespace gaEngineSDK {
  /***************************************************************************/
  /**
  * Inheritance methods.
  */
  /***************************************************************************/
  void
  StaticMesh::update(const float& deltaTime) {
    if (!(m_pModel->m_vAnimationData.empty())) {
      SPtr<AnimationData> currentAnimation = make_shared<AnimationData>();
      currentAnimation = m_pModel->m_vAnimationData[0];

      if (m_playAnimation) {
        m_timeOfAnimation += deltaTime;
      }
      if (m_timeOfAnimation >= currentAnimation->m_animDuration) {
        m_timeOfAnimation = 0.0f;
      }

      uint32 meshNum = 0;

      for (auto& mesh : m_pModel->m_vMeshes) {
        mesh.m_cbModelData = &m_pModel->m_vModelData[meshNum];
        mesh.animated(m_timeOfAnimation, currentAnimation, m_pModel->m_animInfo.m_pStrModelNodes);
        ++meshNum;
      }
    }
  }

  void 
  StaticMesh::render() {
    auto myGraphicApi = g_graphicApi().instancePtr();

    uint32 meshNum = 0;

    for (auto& mesh : m_pModel->m_vMeshes) {
      auto temp = m_pMyActor->getComponent<Transform>();
      
      if (nullptr != temp) {
        auto transform = myGraphicApi->matrixPolicy(temp->getTransform());
        m_pModel->m_vModelData[meshNum].modelMatrix = transform;
      }

      myGraphicApi->updateConstantBuffer(&m_pModel->m_vModelData[meshNum],
                                         myGraphicApi->getConstBufferBones());

      if (!(m_pModel->m_vMeshes[meshNum].m_vTextures.empty())) {
        myGraphicApi->setShaderResourceView(m_pModel->m_vMeshes[meshNum].m_vTextures, 0, 1);
      }

      myGraphicApi->setVertexBuffer(mesh.m_pVertexBuffer);
      myGraphicApi->setIndexBuffer(mesh.m_pIndexBuffer);

      myGraphicApi->drawIndex(mesh.getNumIndices());
      ++meshNum;
    }
  }
}