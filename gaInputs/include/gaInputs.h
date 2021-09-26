#pragma once

#include <gaBaseInputs.h>
#include <gainput/gainput.h>

#include "gaPrerequisitesInputs.h"

namespace gaEngineSDK {
  class Inputs final : public BaseInputs
  {
   public:
    /*************************************************************************/
    /**
    * Constructor and destructor.
    */
    /*************************************************************************/
    Inputs() = default;

    ~Inputs() = default;

    /*************************************************************************/
    /**
    * Inheritance methods.
    */
    /*************************************************************************/
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
    resize(uint32 width, uint32 height) override;

    /*
    * @brief .
    */
    void
    handleMsg(MSG message) override;

    /*************************************************************************/
    /**
    * Keyboard.
    */
    /*************************************************************************/

    /*
    * @brief .
    */
    bool
    getKeyDown(KEYBOARD::E key) override;

    /*
    * @brief .
    */
    bool
    getKey(KEYBOARD::E key) override;

    /*
    * @brief .
    */
    bool
    geyKeyUp(KEYBOARD::E key) override;

    /*************************************************************************/
    /**
    * Mouse.
    */
    /*************************************************************************/

    /*
    * @brief .
    */
    bool
    getMouseBtnDown(MOUSE_BUTTON::E mouseBtn) override;

    /*
    * @brief .
    */
    bool
    getMouseBtn(MOUSE_BUTTON::E mouseBtn) override;

    /*
    * @brief .
    */
    bool
    getMouseBtnUp(MOUSE_BUTTON::E mouseBtn) override;

    /*
    * @brief .
    */
    Vector2I
    getMousePosition() override;

    /*************************************************************************/
    /**
    * Gamepad.
    */
    /*************************************************************************/

    /*
    * @brief .
    */
    Vector2
    getGamepadAxis(GAMEPAD_AXIS::E axis) override;

    /*
    * @brief .
    */
    bool
    getGamepadBtnDown(GAMEPAD_BUTTON::E button) override;

    /*
    * @brief .
    */
    bool
    getGamepadBtn(GAMEPAD_BUTTON::E button) override;

    /*
    * @brief .
    */
    bool
    getGamepadBtnUp(GAMEPAD_BUTTON::E button) override;

   private:
    /*************************************************************************/
    /**
    * Members.
    */
    /*************************************************************************/

    /*
    * @brief .
    */
    gainput::InputManager m_manager;
    
    /*
    * @brief .
    */
    gainput::InputMap* m_mouseBtnMap = nullptr;
    
    /*
    * @brief .
    */
    gainput::InputMap* m_mouseAxisMap = nullptr;
    
    /*
    * @brief .
    */
    gainput::InputMap* m_keyMap = nullptr;
    
    /*
    * @brief .
    */
    gainput::InputMap* m_gamepadBtnMap = nullptr;
    
    /*
    * @brief .
    */
    gainput::InputMap* m_gamepadAxisMap = nullptr;
  };

  /***************************************************************************/
  /**
  * Export.
  */
  /***************************************************************************/

  extern "C" GA_INPUTS_EXPORT BaseInputs *
  newInputs() {
    Inputs* pInputs = new Inputs();
    return pInputs;
  }
}