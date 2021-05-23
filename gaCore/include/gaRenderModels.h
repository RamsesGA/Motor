#pragma once

#include "gaPrerequisitesCore.h"
#include "gaGameObject.h"

namespace gaEngineSDK
{
  class RenderModels : public GameObject
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
      onInit() override {};
    
      /*
      * @brief
      */
      void
      onUpdate(const float& deltaTime) override {};
    
      /*
      * @brief
      */
      void
      onDelete() override {};
    
      /*
      * @brief
      */
      void
      onDraw() override {};
  };
}