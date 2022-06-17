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

    virtual
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

    virtual bool
    startOmni() { return false; };

    virtual void
    shutdownOmni() {};

    virtual void
    connectedUsername(const String& stageUrl) {
      GA_UNREFERENCED_PARAMETER(stageUrl);
    };

    virtual String
    createOmniScene(const String& folderPath, const String& stageName) {
      GA_UNREFERENCED_PARAMETER(folderPath);
      GA_UNREFERENCED_PARAMETER(stageName);
      return String();
    };

    virtual bool
    openUSD(const String& folderPath, const String& stageName) {
      GA_UNREFERENCED_PARAMETER(folderPath);
      GA_UNREFERENCED_PARAMETER(stageName);
      return false;
    };

    virtual bool
    loadUSD() {
      return false; 
    };

    /*
    * @brief SG = SceneGraph.
    */
    virtual void
    saveSGToUSD() {};

    virtual void
    updateGaToOmni() {};

    virtual void
    updateOmniToGa() {};

    virtual bool
    getIsStartUp() { return false; };

    virtual bool
    getOmniverseLogging() { return false; };

    virtual bool
    getIsLiveSync() { return false; };

    virtual String
    getCurrentURL() { return String(); };

    virtual void
    setIsLiveSync(bool& isLive) {
      GA_UNREFERENCED_PARAMETER(isLive);
    };

    virtual void
    setOmniverseLog(bool& omniverseLog) {
      GA_UNREFERENCED_PARAMETER(omniverseLog);
    };
  };

  /***************************************************************************/
  /**
  * INSTANCE.
  */
  /***************************************************************************/

  GA_CORE_EXPORT BaseOmniConnect&
  g_baseOmniConnect();
}