#pragma once

#include <gaModule.h>

#include "gaPrerequisitesCore.h"

namespace gaEngineSDK {
  class GA_CORE_EXPORT BaseOmniConnect : public Module<BaseOmniConnect>
  {
   public:
    /*************************************************************************/
    /**
    * Constructor and destructor.
    */
    /*************************************************************************/

    BaseOmniConnect() = default;

    ~BaseOmniConnect() = default;

    /*
    * @brief .
    */
    void
    setObject(BaseOmniConnect* api) {
      BaseOmniConnect::_instance() = api;
    }

    /*************************************************************************/
    /**
    * Omniverse virtual Methods.
    */
    /*************************************************************************/

    /*
    * @brief Startup Omniverse.
    */
    virtual bool
    startOmniverse() { return false; };

    /*
    * @brief Create a scene with format ".usd".
    */
    virtual String
    createOmniverseScene(const String& destinationPath, const String& stageName) { 
      return String();
    };

    /*
    * @brief Print the current user online.
    */
    virtual void
    printConnectedUsername(const String& stageUrl) {};

  };

  /***************************************************************************/
  /**
  * INSTANCE.
  */
  /***************************************************************************/

  GA_CORE_EXPORT BaseOmniConnect&
  g_baseOmniConnect();
}