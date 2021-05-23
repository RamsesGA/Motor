#pragma once

#include "gaPrerequisitesCore.h"

namespace gaEngineSDK
{
  class ResourceManager
  {
    public:
      /***********************************************************************/
      /**
      * Constructor and destructor.
      */
      /***********************************************************************/
      ResourceManager() = default;
    
      ~ResourceManager() = default;
    
      /***********************************************************************/
      /**
      * Methods.
      */
      /***********************************************************************/
    
      /*
      * @brief
      */
      void
      onInit() {};
    
      /*
      * @brief
      */
      void
      onUpdate(const float& deltaTime) {};
    
      /*
      * @brief
      */
      void
      onDelete() {};

      /*
      * @brief
      */
      void
      onRelease() {};
  };
}