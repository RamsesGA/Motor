#pragma once

#include <tchar.h>
#include <gaVector2I.h>
#include <SFML/Window.hpp>

#include "gaPrerequisitesCore.h"

using sf::Window;
using sf::VideoMode;
using sf::WindowHandle;

namespace gaEngineSDK {

  class GA_CORE_EXPORT BaseApp
  {
    public:
      /***********************************************************************/
      /*
      * Constructor and destructor.
      */
      /***********************************************************************/
      BaseApp() = default;

      virtual 
      ~BaseApp() = default;

      /***********************************************************************/
      /*
      * Methods.
      */
      /***********************************************************************/

      int32
      run(String windowTitle, int32 sizeX = 1024, int32 sizeY = 768);

    protected:
      void
      createWindow(String windowTitle);

      void
      init();

      void
      update(float deltaTime);

      void
      render();

      void
      create();

      void
      destroySystem();

      void
      resize(int32 width, int32 height);

      /***********************************************************************/
      /*
      * On Methods (virtual).
      */
      /***********************************************************************/

      virtual int32
      onInit() { return 0; };

      virtual void
      onInitCamera(bool isOGL = false) {};

      virtual void
      onUpdate(float deltaTime) {};

      virtual void
      onRender() {};

      virtual void
      onCreate() {};

      virtual void
      onDestroySystem() {};

    protected:
      /***********************************************************************/
      /*
      * Members.
      */
      /***********************************************************************/

      //Tamaño de pantalla
      unsigned int m_width;
      unsigned int m_height;

      Vector2I m_windowSize;
      Vector2I m_windowPosition;

      String m_windowTitle;

      Window m_sfmlWindow;
  };
}