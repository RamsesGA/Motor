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

      /***********************************************************************/
      /*
      * Members.
      */
      /***********************************************************************/

      uint32 m_numMaterials = 0;

      /*
      * @brief
      */
      bool m_hasDiffuse = false;

      /*
      * @brief
      */
      bool m_hasAlbedo = false;

      /*
      * @brief
      */
      bool m_hasNormal = false;

      /*
      * @brief
      */
      bool m_hasSpecular = false;

      /*
      * @brief
      */
      bool m_hasMetalness = false;

      /*
      * @brief
      */
      bool m_hasRoughness = false;

      /*
      * @brief
      */
      bool m_hasEmissive = false;

      /*
      * @brief
      */
      bool m_hasAO = false;

      /*
      * @brief Member to save the info of this type of texture.
      */
      Textures m_diffuse;

      /*
      * @brief Member to save the info of this type of texture.
      */
      Textures m_albedo;

      /*
      * @brief Member to save the info of this type of texture.
      */
      Textures m_normals;

      /*
      * @brief Member to save the info of this type of texture.
      */
      Textures m_specular;

      /*
      * @brief Member to save the info of this type of texture.
      */
      Textures m_metalness;

      /*
      * @brief Member to save the info of this type of texture.
      */
      Textures m_roughness;
  };
}