#pragma once

#include "gaPrerequisitesCore.h"

namespace gaEngineSDK {

  namespace TYPE_COMPONENTS {
    enum E {
      kRenderModel = 0,
      kCamera,
      kTexture,
      kUndefined
    };
  };

  class Camera;
  class Textures;
  class ResourceManager;

  class GA_CORE_EXPORT Component
  {
    public:
      /***********************************************************************/
      /**
      * Constructor and destructor.
      */
      /***********************************************************************/

      Component() = default;

      Component(WeakSPtr<ResourceManager> modelInfo);
      
      ~Component() = default;

      /***********************************************************************/
      /**
      * Sets.
      */
      /***********************************************************************/

    protected:
      /***********************************************************************/
      /**
      * Members.
      */
      /***********************************************************************/

      TYPE_COMPONENTS::E m_typeComponent = TYPE_COMPONENTS::kUndefined;

      /*
      * @brief .
      */
      SPtr<ResourceManager> m_modelInfo = nullptr;
  };
}