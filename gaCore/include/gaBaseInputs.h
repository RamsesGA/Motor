#pragma once

#include <gaModule.h>
#include <gaVector2.h>
#include <gaVector2I.h>
#include <Windows.h>

#include "gaPrerequisitesCore.h"

namespace gaEngineSDK {
  namespace KEYBOARD {
    enum E {
      kQ,
      kW,
      kE,
      kA,
      kS,
      kD,
      kZ,
      kC,
      kUP,
      kDOWN,
      kLEFT,
      kRIGHT
    };
  }

  namespace MOUSE_BUTTON {
    enum E {
      kLeft,
      kRight,
      kMiddle
    };
  }

  namespace MOUSE_AXIS {
    enum E {
      kX,
      kY
    };
  }

  namespace GAMEPAD_BUTTON {
    enum E {
      kStart
    };
  }

  namespace GAMEPAD_AXIS {
    enum E {
      kBtnLeftStickX,
      kBtnLeftStickY,
    };
  }

  class GA_CORE_EXPORT BaseInputs : public Module<BaseInputs>
  {
   public:
    /*************************************************************************/
    /**
    * Constructor and destructor.
    */
    /*************************************************************************/

    BaseInputs() = default;

    ~BaseInputs() = default;

    /*
    * @brief .
    */
    void
    setObject(BaseInputs* api) {
      BaseInputs::_instance() = api;
    }

    /*************************************************************************/
    /**
    * Basic methods.
    */
    /*************************************************************************/

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
    resize(uint32 width, uint32 height) {};

    /*
    * @brief .
    */
    virtual void
    handleMsg(MSG message) {};

    /*************************************************************************/
    /**
    * Keyboard.
    */
    /*************************************************************************/

    /*
    * @brief .
    */
    virtual bool
    getKeyDown(KEYBOARD::E key) { return false; };

    /*
    * @brief .
    */
    virtual bool
    getKey(KEYBOARD::E key) { return false; };

    /*
    * @brief .
    */
    virtual bool
    geyKeyUp(KEYBOARD::E key) { return false; };

    /*************************************************************************/
    /**
    * Mouse.
    */
    /*************************************************************************/

    /*
    * @brief .
    */
    virtual bool
    getMouseBtnDown(MOUSE_BUTTON::E mouseBtn) { return false; };

    /*
    * @brief .
    */
    virtual bool
    getMouseBtn(MOUSE_BUTTON::E mouseBtn) { return false; };

    /*
    * @brief .
    */
    virtual bool
    getMouseBtnUp(MOUSE_BUTTON::E mouseBtn) { return false; };

    /*
    * @brief .
    */
    virtual Vector2I
    getMousePosition() { return Vector2I(); };

    /*************************************************************************/
    /**
    * Gamepad.
    */
    /*************************************************************************/

    /*
    * @brief .
    */
    virtual Vector2
    getGamepadAxis(GAMEPAD_AXIS::E axis) { return Vector2(); };

    /*
    * @brief .
    */
    virtual bool
    getGamepadBtnDown(GAMEPAD_BUTTON::E button) { return false; };

    /*
    * @brief .
    */
    virtual bool
    getGamepadBtn(GAMEPAD_BUTTON::E button) { return false; };

    /*
    * @brief .
    */
    virtual bool
    getGamepadBtnUp(GAMEPAD_BUTTON::E button) { return false; };
  };

  /***************************************************************************/
  /**
  * INSTANCE.
  */
  /***************************************************************************/

  GA_CORE_EXPORT BaseInputs&
  g_baseInputs();
}