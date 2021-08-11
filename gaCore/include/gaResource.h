#pragma once

#include "gaPrerequisitesCore.h"

namespace gaEngineSDK
{
  namespace RESOURCE_TYPES {
    enum E {
      kUnknown = 0,
      kModel,
      kTexture,
      //TODO: AÑADIR MAS TIPOS DE RECURSOS
    };
  }

  class GA_CORE_EXPORT Resource
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

      virtual void
      loadFromFile(const String& file) = 0;
  };
}