#pragma once

#include <gaVector2I.h>
#include <SFML/Window.hpp>

#include "gaPrerequisitesCore.h"

using sf::Event;
using sf::Window;
using sf::VideoMode;
using sf::WindowHandle;

namespace gaEngineSDK {

  class BaseApp
  {
    public:
      BaseApp() = default;

      virtual 
      ~BaseApp() = default;

      int32
      run();

    protected:
      virtual void
      init();

      virtual void
      update(float deltaTime);

      virtual void
      render();

      virtual void
      create();

      virtual void
      createWindow();

      virtual void
      resize(int32 width, int32 height);

      virtual void
      destroySystem();

      Vector2I m_windowPosition;
      Vector2I m_windowSize;

      std::string m_windoTitle;

      Window m_sfmlWindow;
  };
}