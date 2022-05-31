#pragma once

#include <gaModule.h>

#include "gaPrerequisitesCore.h"

namespace gaEngineSDK {
  class RenderTarget;

  class GA_CORE_EXPORT BaseInterface : public Module<BaseInterface>
  {
   public:
    /*************************************************************************/
    /**
    * Constructor and destructor.
    */
    /*************************************************************************/

    BaseInterface() = default;

    ~BaseInterface() = default;

    /*
    * @brief .
    */
    void
    setObject(BaseInterface* api) {
      BaseInterface::_instance() = api;
    }

    /*
    * @brief .
    */
    virtual void
    init(uint32 width, uint32 height, void* hwndHandle) {};

    /*
    * @brief .
    */
    virtual void
    update(const float& deltaTime) {};

    /*
    * @brief .
    */
    virtual void
    render() {};

    /*
    * @brief .
    */
    virtual void
    mouseButtonDown(uint32 button) {};

    /*
    * @brief .
    */
    virtual void
    mouseButtonReleased(uint32 button) {};

    /*
    * @brief .
    */
    virtual void
    textEnter(wchar_t unicode) {};

    virtual void
    descompressImages() {};

    bool m_touchingImGui = false;

    void* m_windowsHandle = nullptr;

    bool m_startOmniverse = false;

    bool m_isRunningOmniverse = false;
  };

  /***************************************************************************/
  /**
  * INSTANCE.
  */
  /***************************************************************************/

  GA_CORE_EXPORT BaseInterface&
  g_baseInterface();
}