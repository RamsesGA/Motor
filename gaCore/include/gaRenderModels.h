#pragma once

#include "gaPrerequisitesCore.h"
#include "gaGameObject.h"
#include "gaModels.h"
#include "gaStructures.h"
#include "gaConstantBuffer.h"

namespace gaEngineSDK {

  struct ConstBuffBonesTransform::E;

  class GA_CORE_EXPORT RenderModels : public GameObject
  {
    public:
      /***********************************************************************/
      /**
      * Constructor and destructor.
      */
      /***********************************************************************/
      RenderModels() = default;
    
      ~RenderModels() = default;
    
      /***********************************************************************/
      /**
      * Methods.
      */
      /***********************************************************************/
    
      /*
      * @brief
      */
      void
      init(WeakSPtr<Model> model);
    
      /*
      * @brief
      */
      void
      onUpdate(const float& deltaTime) override;
    
      /*
      * @brief
      */
      void
      drawModel(ResourceManager& resource, WeakSPtr<ConstantBuffer> cbBone);

      void
      changeModel(uint32 numMeshNewMode);

      void
      noneAnimation();

    private:
      /***********************************************************************/
      /**
      * Members.
      */
      /***********************************************************************/

      bool m_playAnimation = true;

      float m_timeOfAnimation = 0.0f;

      Vector<ConstBuffBonesTransform::E> m_meshBones;

      SPtr<Model> m_pModel = nullptr;

      SPtr<AnimationData> m_currentAnimation = nullptr;
  };
}