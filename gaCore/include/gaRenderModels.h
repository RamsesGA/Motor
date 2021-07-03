#pragma once

#include "gaPrerequisitesCore.h"
#include "gaModels.h"
#include "gaStructures.h"
#include "gaConstantBuffer.h"
#include "gaComponent.h"

namespace gaEngineSDK {

  class GA_CORE_EXPORT RenderModels : public Component
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
      update(ResourceManager& resource, const float& deltaTime);
    
      /*
      * @brief .
      */
      void
      drawModel(WeakSPtr<ResourceManager> resource, WeakSPtr<ConstantBuffer> cbBone);

      /*
      * @brief .
      */
      void
      setMeshBones(ResourceManager& resource);

      /*
      * @brief .
      */
      SPtr<AnimationData> m_currentAnimation = nullptr;

    private:
      /***********************************************************************/
      /**
      * Members.
      */
      /***********************************************************************/

      /*
      * @brief .
      */
      bool m_playAnimation = true;

      /*
      * @brief .
      */
      float m_timeOfAnimation = 0.0f;

      /*
      * @brief .
      */
      Vector<ConstBuffBonesTransform> m_meshBones;

      /*
      * @brief .
      */
      SPtr<ResourceManager> m_pResourceMang = nullptr;
  };
}