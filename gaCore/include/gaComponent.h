#pragma once

#include "gaPrerequisitesCore.h"

namespace gaEngineSDK {
  class GA_CORE_EXPORT Component
  {
    public:
      /***********************************************************************/
      /**
      * Constructor and destructor.
      */
      /***********************************************************************/

      Component() = default;

      ~Component() = default;

      /***********************************************************************/
      /**
      * Methods.
      */
      /***********************************************************************/

      virtual void
      update(const float& deltaTime) { };

      virtual void
      render() { };
  };
}