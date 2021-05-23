#pragma once

#include "gaPrerequisitesCore.h"
#include "gaGameObject.h"

namespace gaEngineSDK
{
  class Transform : public GameObject
  {
    public:
      /*************************************************************************/
      /**
      * Constructor and destructor.
      */
      /*************************************************************************/
      Transform() = default;
    
      ~Transform() = default;
    
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