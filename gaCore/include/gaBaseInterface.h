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
    init(uint32 width, uint32 height, void* hwndHandle) {
      GA_UNREFERENCED_PARAMETER(width);
      GA_UNREFERENCED_PARAMETER(height);
      GA_UNREFERENCED_PARAMETER(hwndHandle);
    };

    /*
    * @brief .
    */
    virtual void
    update(const float& deltaTime) {
      GA_UNREFERENCED_PARAMETER(deltaTime);
    };

    /*
    * @brief .
    */
    virtual void
    render() {};

    virtual void
    descompressImages() {};

    /*************************************************************************/
    /**
    * Inputs for ImGui.
    */
    /*************************************************************************/

    /*
    * @brief .
    */
    virtual void
    mouseButtonDown(uint32 button) {
      GA_UNREFERENCED_PARAMETER(button);
    };

    /*
    * @brief .
    */
    virtual void
    mouseButtonReleased(uint32 button) {
      GA_UNREFERENCED_PARAMETER(button);
    };

    /*
    * @brief .
    */
    virtual void
    textEnter(wchar_t unicode) {
      GA_UNREFERENCED_PARAMETER(unicode);
    };

    virtual void
    onKeyPressed(bool alt, bool ctrl, bool shift, bool sys, int32 code) {
      GA_UNREFERENCED_PARAMETER(alt);
      GA_UNREFERENCED_PARAMETER(ctrl);
      GA_UNREFERENCED_PARAMETER(shift);
      GA_UNREFERENCED_PARAMETER(sys);
      GA_UNREFERENCED_PARAMETER(code);
    };

    virtual void
    onKeyReleased(bool alt, bool ctrl, bool shift, bool sys, int32 code) {
      GA_UNREFERENCED_PARAMETER(alt);
      GA_UNREFERENCED_PARAMETER(ctrl);
      GA_UNREFERENCED_PARAMETER(shift);
      GA_UNREFERENCED_PARAMETER(sys);
      GA_UNREFERENCED_PARAMETER(code);
    };

    /*************************************************************************/
    /**
    * Members.
    */
    /*************************************************************************/

    bool m_touchingImGui = false;

    void* m_windowsHandle = nullptr;
  };

  /***************************************************************************/
  /**
  * INSTANCE.
  */
  /***************************************************************************/

  GA_CORE_EXPORT BaseInterface&
  g_baseInterface();
}