#pragma once

#include <SFML/Window.hpp>
#include <gaVector2I.h>

#include "gaPrerequisitesCore.h"

using sf::Window;
using sf::VideoMode;
using sf::WindowHandle;

namespace gaEngineSDK {

  class GA_CORE_EXPORT BaseApp
  {
    public:
      BaseApp() = default;

      virtual 
      ~BaseApp() = default;

      int32
      run();

    protected:
      void
      createWindow(int32 sizeX, int32 sizeY, String windowTitle);

      virtual void
      init(){};

      virtual void
      onInit() {};

      virtual void
      update(float deltaTime) {};

      virtual void
      onUpdate(float deltaTime) {};

      virtual void
      render() {};

      virtual void
      onRender() {};

      virtual void
      create() {};

      virtual void
      onCreate() {};

      virtual void
      resize(int32 width, int32 height) {};

      virtual void
      destroySystem() {};

      Vector2I m_windowSize;

      std::string m_windowTitle;

      Vector2I m_windowPosition;

      Window m_sfmlWindow;
  };
}