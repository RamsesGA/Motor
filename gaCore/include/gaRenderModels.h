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
      drawModel();

      /***********************************************************************/
      /**
      * Sets.
      */
      /***********************************************************************/

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
      SPtr<ResourceManager> m_pResource = nullptr;

      /*
      * @brief .
      */
      Vector<ConstBuffBonesTransform> m_meshBones;
  };
}