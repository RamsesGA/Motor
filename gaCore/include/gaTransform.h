#pragma once

#include "gaPrerequisitesCore.h"
#include "gaGameObject.h"

namespace gaEngineSDK
{
  class Transform final : public GameObject
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
      update(const float& deltaTime) override {};

      /*
      * @brief
      */
      void
      draw() override {};

  };
}