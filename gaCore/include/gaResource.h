#pragma once

#include "gaPrerequisitesCore.h"

namespace gaEngineSDK
{
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

      void
      setId(int32 id = 0);

      int32
      getId();

      void
      setRSRCName(String name);

      String
      getRSRCName();

    private:
      /***********************************************************************/
      /**
      * Members.
      * TODO: tipo del recurso que va a obtener
      */
      /***********************************************************************/

      int32 m_id;

      String m_RSRCName;

  };
}