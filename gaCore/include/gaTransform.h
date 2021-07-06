#pragma once

#include <gaVector3.h>
#include <gaMatrix4x4.h>

#include "gaPrerequisitesCore.h"
#include "gaComponent.h"

namespace gaEngineSDK {
  class GA_CORE_EXPORT Transform final : public Component
  {
    public:
      /*************************************************************************/
      /**
      * Constructor and destructor.
      */
      /*************************************************************************/
      Transform();
    
      ~Transform() = default;
    
      /***********************************************************************/
      /**
      * Methods.
      */
      /***********************************************************************/

      /*
      * @brief .
      */
      void
      update(const float& deltatime) override;

    private:
      /***********************************************************************/
      /**
      * Members.
      */
      /***********************************************************************/

      /*
      * @brief .
      */
      Vector3 m_position = {0.0f, 0.0f, 0.0f};

      /*
      * @brief .
      */
      Vector3 m_rotation = { 0.0f, 0.0f, 0.0f };

      /*
      * @brief .
      */
      Vector3 m_scale = { 0.0f, 0.0f, 0.0f };

      friend class Actor;
  };
}