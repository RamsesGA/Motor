#pragma once

#include "gaPrerequisitesCore.h"
#include "gaGameObject.h"

namespace gaEngineSDK
{
  class Actor final : public GameObject
  {
    public:
      /***********************************************************************/
      /**
      * Constructor and destructor.
      */
      /***********************************************************************/
      Actor() = default;

      ~Actor() = default;
  };
}