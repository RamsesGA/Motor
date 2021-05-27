#pragma once

#include "gaPrerequisitesCore.h"

namespace gaEngineSDK
{
  namespace RESOURCE_TYPE {
    enum E {
      UNKNOWN = 0,
      MODEL,
      TEXTURE,
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
      * Methods.
      */
      /***********************************************************************/
    
      /*
      * @brief
      */
      virtual void
      onInit() {};
    
      /*
      * @brief
      */
      virtual void
      onUpdate(const float& deltaTime) {};
    
      /*
      * @brief
      */
      virtual void
      onDelete() {};
    
      /*
      * @brief
      */
      virtual void
      onRelease() {};

      /***********************************************************************/
      /**
      * Sets and Gets.
      */
      /***********************************************************************/

      /*
      * @brief Method to set the id.
      */
      void
      setId(int32 id = 0);

      /*
      * @brief Method to get the id.
      */
      int32
      getId();

      /*
      * @brief Method to set the resource name.
      */
      void
      setRSRCName(const String& name = "default");

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
      int32 m_id;

      /*
      * @brief Resource member name.
      */
      String m_RSRCName;
  };
}