#pragma once

#include <gaBaseInterface.h>

#include "gaPrerequisitesInterface.h"

namespace gaEngineSDK {
  class Interface final : public BaseInterface
  {
   public:
    /*************************************************************************/
    /**
    * Constructor and destructor.
    */
    /*************************************************************************/

    Interface() = default;

    ~Interface() = default;

    /*
    * @brief .
    */
    void
    init(uint32 width, uint32 height, void* hwndHandle) override;

    /*
    * @brief .
    */
    void
    update(const float& deltaTime) override;

    /*
    * @brief .
    */
    void
    render() override;

    /*************************************************************************/
    /**
    * Inputs for ImGui.
    */
    /*************************************************************************/

    void
    mouseButtonDown(uint32 button) override;

    void
    mouseButtonReleased(uint32 button) override;

    void
    textEnter(wchar_t unicode) override;

    /*************************************************************************/
    /**
    * Methods for ImGui.
    */
    /*************************************************************************/

    void 
    menuBarFiles();

    /*************************************************************************/
    /**
    * Members.
    */
    /*************************************************************************/

    /*
    * @brief .
    */
    uint32 m_width = 0;

    /*
    * @brief .
    */
    uint32 m_height = 0;

    String m_text;

    float m_size;
  };

  /***************************************************************************/
  /**
  * Export.
  */
  /***************************************************************************/

  extern "C" GA_INTERFACE_EXPORT BaseInterface *
  createNewInterface() {
    Interface* pInterface = new Interface();
    return pInterface;
  }
}