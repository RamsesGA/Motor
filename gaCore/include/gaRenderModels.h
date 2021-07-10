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

      /*
      * @brief .
      */
      void
      changeModel(uint32 numMesh);

      /***********************************************************************/
      /**
      * Sets.
      */
      /***********************************************************************/

      /*
      * @brief .
      */
      void
      setModel(WeakSPtr<Model> model);

      /*
      * @brief .
      */
      SPtr<AnimationData> m_currentAnimation = nullptr;

      /*
      * @brief .
      */
      Vector<ConstBuffBonesTransform> m_meshBones;

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
      uint32 m_animationNumber = 0;

      /*
      * @brief .
      */
      float m_timeOfAnimation = 0.0f;

      /*
      * @brief .
      */
      SPtr<Model> m_model = nullptr;

      /*
      * @brief .
      */
      SPtr<ResourceManager> m_pResource = nullptr;

      
  };
}