#pragma once

#include "gaPrerequisitesCore.h"
#include "gaComponent.h"
#include "gaTextures.h"

namespace gaEngineSDK {

  class GA_CORE_EXPORT Materials : public Component
  {
    public:
      /***********************************************************************/
      /*
      * Constructor and destructor.
      */
      /***********************************************************************/
      Materials() = default;
      
      ~Materials() = default;

      void
      update(const float& deltaTime)override;

      void
      render()override;

    private:
      /***********************************************************************/
      /*
      * Members.
      */
      /***********************************************************************/

      String m_materialName = "Default_M";

      SPtr<Textures> m_textureInfo = nullptr;

      Vector<SPtr<Materials>> m_vMaterials;
  };
}