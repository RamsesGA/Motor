#include "gaInputs.h"

namespace gaEngineSDK {
  /***************************************************************************/
  /**
  * Basic methods.
  */
  /***************************************************************************/
  void
  Inputs::init(uint32 width, uint32 height, void* hwndHandle) {
    gainput::DeviceId mouseId   = m_manager.CreateDevice<gainput::InputDeviceMouse>();
    gainput::DeviceId keybordId = m_manager.CreateDevice<gainput::InputDeviceKeyboard>();
    gainput::DeviceId padId     = m_manager.CreateDevice<gainput::InputDevicePad>();

    m_manager.SetDisplaySize(width, height);

    //Init own members
    m_mouseBtnMap    = new gainput::InputMap(m_manager);
    m_mouseAxisMap   = new gainput::InputMap(m_manager);
    m_keyMap         = new gainput::InputMap(m_manager);
    m_gamepadBtnMap  = new gainput::InputMap(m_manager);
    m_gamepadAxisMap = new gainput::InputMap(m_manager);

    //Mapping keyboard
    m_keyMap->MapBool(KEYBOARD::kW, keybordId, gainput::KeyW);
    m_keyMap->MapBool(KEYBOARD::kS, keybordId, gainput::KeyS);
    m_keyMap->MapBool(KEYBOARD::kA, keybordId, gainput::KeyA);
    m_keyMap->MapBool(KEYBOARD::kD, keybordId, gainput::KeyD);
    m_keyMap->MapBool(KEYBOARD::kQ, keybordId, gainput::KeyQ);
    m_keyMap->MapBool(KEYBOARD::kE, keybordId, gainput::KeyE);
    m_keyMap->MapBool(KEYBOARD::kZ, keybordId, gainput::KeyZ);
    m_keyMap->MapBool(KEYBOARD::kC, keybordId, gainput::KeyC);
    m_keyMap->MapBool(KEYBOARD::kUP, keybordId, gainput::KeyUp);
    m_keyMap->MapBool(KEYBOARD::kDOWN, keybordId, gainput::KeyDown);
    m_keyMap->MapBool(KEYBOARD::kLEFT, keybordId, gainput::KeyLeft);
    m_keyMap->MapBool(KEYBOARD::kRIGHT, keybordId, gainput::KeyRight);

    //Mapping mouse buttons
    m_mouseBtnMap->MapBool(MOUSE_BUTTON::kLeft, mouseId, gainput::MouseButtonLeft);
    m_mouseBtnMap->MapBool(MOUSE_BUTTON::kRight, mouseId, gainput::MouseButtonRight);

    //Mapping mouse axis
    m_mouseAxisMap->MapFloat(MOUSE_AXIS::kX, mouseId, gainput::MouseAxisX);
    m_mouseAxisMap->MapFloat(MOUSE_AXIS::kY, mouseId, gainput::MouseAxisY);

    //Mapping pad
    m_gamepadBtnMap->MapBool(GAMEPAD_BUTTON::kStart, padId, gainput::PadButtonStart);

    //Mapping axis
    m_gamepadAxisMap->MapFloat(GAMEPAD_AXIS::kBtnLeftStickX, 
                               padId, 
                               gainput::PadButtonLeftStickX);
  }

  void
  Inputs::update(const float& deltaTime) {
    m_manager.Update();
  }

  void 
  Inputs::resize(uint32 width, uint32 height) {
    m_manager.SetDisplaySize(width, height);
  }

  void 
  Inputs::handleMsg(MSG message) {
    MSG msg = tagMSG(message);

    m_manager.HandleMessage(msg);
  }

  /***************************************************************************/
  /**
  * Keyboard.
  */
  /***************************************************************************/

  bool 
  Inputs::getKeyDown(KEYBOARD::E key) {
    return m_keyMap->GetBoolIsNew(key);
  }

  bool
  Inputs::getKey(KEYBOARD::E key) {
    return m_keyMap->GetBool(key);
  }

  bool 
  Inputs::geyKeyUp(KEYBOARD::E key) {
    return m_keyMap->GetBoolWasDown(key);
  }

  /***************************************************************************/
  /**
  * Mouse.
  */
  /***************************************************************************/

  bool
  Inputs::getMouseBtnDown(MOUSE_BUTTON::E mouseBtn) {
    return m_mouseBtnMap->GetBoolIsNew(mouseBtn);
  }

  bool 
  Inputs::getMouseBtn(MOUSE_BUTTON::E mouseBtn) {
    return m_mouseBtnMap->GetBool(mouseBtn);
  }

  bool 
  Inputs::getMouseBtnUp(MOUSE_BUTTON::E mouseBtn) {
    return m_mouseBtnMap->GetBoolWasDown(mouseBtn);
  }

  Vector2I 
  Inputs::getMousePosition() {
    uint32 x = m_mouseAxisMap->GetFloat(MOUSE_AXIS::kX) * m_manager.GetDisplayWidth();
    uint32 y = m_mouseAxisMap->GetFloat(MOUSE_AXIS::kY) * m_manager.GetDisplayHeight();
    return Vector2I(x, y);
  }

  /***************************************************************************/
  /**
  * Gamepad.
  */
  /***************************************************************************/

  Vector2 
  Inputs::getGamepadAxis(GAMEPAD_AXIS::E axis) {
    return m_gamepadAxisMap->GetFloat(axis);
  }

  bool
  Inputs::getGamepadBtnDown(GAMEPAD_BUTTON::E button) {
    return m_gamepadBtnMap->GetBoolIsNew(button);
  }

  bool
  Inputs::getGamepadBtn(GAMEPAD_BUTTON::E button) {
    return m_gamepadBtnMap->GetBool(button);
  }

  bool 
  Inputs::getGamepadBtnUp(GAMEPAD_BUTTON::E button) {
    return m_gamepadBtnMap->GetBoolWasDown(button);
  }
}