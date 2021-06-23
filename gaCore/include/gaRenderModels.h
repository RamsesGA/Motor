#pragma once

#include "gaPrerequisitesCore.h"
#include "gaGameObject.h"
#include "gaModels.h"
#include "gaStructures.h"
#include "gaConstantBuffer.h"

namespace gaEngineSDK {

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
      update(ResourceManager& resource, const float& deltaTime);
    
      /*
      * @brief
      */
      void
      drawModel(WeakSPtr<ResourceManager> resource, WeakSPtr<ConstantBuffer> cbBone);

      void
      setMeshBones(ResourceManager& resource);

      SPtr<AnimationData> m_currentAnimation = nullptr;

    private:
      /***********************************************************************/
      /**
      * Members.
      */
      /***********************************************************************/

      bool m_playAnimation = true;

      float m_timeOfAnimation = 0.0f;

      Vector<ConstBuffBonesTransform> m_meshBones;

      SPtr<ResourceManager> m_pResourceMang = nullptr;

  };
}