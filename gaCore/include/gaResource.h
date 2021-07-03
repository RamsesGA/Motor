#pragma once

#include "gaPrerequisitesCore.h"

namespace gaEngineSDK
{
  namespace RESOURCE_TYPES {
    enum E {
      kUnknown = 0,
      kModel,
      kTexture,
      kNumResourcesTypes
      //TODO: AÑADIR MAS TIPOS DE RECURSOS
    };
  }

  class Resource
  {
    public:
      /***********************************************************************/
      /**
      * Constructor and destructor.
      */
      /***********************************************************************/
      Resource() = default;
    
      ~Resource() = default;
    
      /***********************************************************************/
      /**
      * Sets.
      */
      /***********************************************************************/

      /*
      * @brief Method to set the id.
      */
      void
      setId(int32 id = 0);

      /*
      * @brief Method to set the resource name.
      */
      void
      setRSRCName(const String& name = "default");

      /***********************************************************************/
      /**
      * Gets.
      */
      /***********************************************************************/

      /*
      * @brief Method to get the id.
      */
      int32
      getId();

      /*
      * @brief Method to get the rtesource name.
      */
      String
      getRSRCName();

    private:
      /***********************************************************************/
      /**
      * Members.
      */
      /***********************************************************************/

      /*
      * @brief Member to save the id.
      */
      int32 m_id = 0;

      /*
      * @brief Resource member name.
      */
      String m_RSRCName;

      /*
      * @brief Resource member type.
      */
      RESOURCE_TYPES::E m_RSRCType = RESOURCE_TYPES::kUnknown;

      friend class ResourceManager;
  };
}