#pragma once

#include "gaPrerequisitesCore.h"
#include "gaGameObject.h"

namespace gaEngineSDK
{

  class Model;

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

    private:

      Model* m_pModel = nullptr;
  };
}