#pragma once

#include "gaPrerequisitesCore.h"

namespace gaEngineSDK {
  namespace TYPE_TEXTURES {
    enum E {
      kDiffuse = 0,
      kAlbedo,
      kMetallic,
      kNormal,
      kRoughness,
      kSpecular,
      kAO,
      kEmissive
    };
  }

  class GA_CORE_EXPORT Resource
  {
   public:
    /*************************************************************************/
    /**
    * Constructor and destructor.
    */
    /*************************************************************************/
    Resource() = default;

    ~Resource() = default;

    /*************************************************************************/
    /**
    * Methods.
    */
    /*************************************************************************/

    virtual void
    loadFromFile(const String& file) = 0;

    virtual void
    loadFromFile(const String& file, TYPE_TEXTURES::E typeTexture) = 0;
  };
}