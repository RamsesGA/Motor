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

      /*
      * @brief Run is the function that starts the 
      *        entire creation and initialization process.
      */
      int32
      run(String windowTitle, int32 sizeX = 1440, int32 sizeY = 900);

    protected:

      /*
      * @param Create the information for the window.
      */
      void
      createWindow(String windowTitle);

      /*
      * @param Command to call the onInit function.
      */
      void
      init();

      /*
      * @param Command to call the onUpdate function.
      */
      void
      update(float deltaTime);

      /*
      * @param Command to call the onRender function.
      */
      void
      render();

      /*
      * @param Command to call the onCreate function.
      */
      void
      create();

      /*
      * @param Command to call the onDestroySystem function.
      */
      void
      destroySystem();

      /*
      * @param Command to call the onResize function.
      */
      void
      resize(int32 width, int32 height);

      /*
      * @param Command to call the onKeyboardDown function.
      */
      void
      keyboardDown(sf::Event param);

      /*
      * @param Command to call the onLeftMouseBtnDown function.
      */
      void
      leftMouseBtnDown();

      /*
      * @param Command to call the onLeftMouseBtnUp function.
      */
      void
      leftMouseBtnUp();

      /*
      * @param Command to call the onMouseMove function.
      */
      void
      mouseMove();

      /***********************************************************************/
      /*
      * On Methods (virtual).
      */
      /***********************************************************************/

      /*
      * @brief Load the DLL and initialize the graphic api.
      * @return Returns an integer to confirm its initialization.
      */
      virtual int32
      onInit() { return 0; };

      /*
      * @brief Initialize the camera and all its default values.
      * @param Bool to indicate if you are using OGL (this will be removed).
      */
      virtual void
      onInitCamera(bool isOGL = false) {};

      /*
      * @brief Update the constant buffer of the api.
      * @param float for delta time.
      */
      virtual void
      onUpdate(float deltaTime) {};

      /*
      * @brief Save new data, update other and clean old data.
      */
      virtual void
      onRender() {};

      /*
      * @brief Create the data for the api and
      *        confirm that everything exists without problem.
      */
      virtual void
      onCreate() {};

      /*
      * @brief Delete variables to end the program.
      */
      virtual void
      onDestroySystem() {};

      /*
      * @brief Detect keyboard input.
      * @param SFML event to know the input pressed.
      */
      virtual void
      onKeyboardDown(sf::Event param) {};

      /*
      * @brief Indicates if the left mouse button is pressed.
      */
      virtual void
      onLeftMouseBtnDown() {};

      /*
      * @brief Indicates whether the left mouse button was lifted.
      */
      virtual void
      onLeftMouseBtnUp() {};

      /*
      * @brief Detects if the mouse is moving on the screen.
      */
      virtual void
      onMouseMove() {};

    protected:
      /***********************************************************************/
      /*
      * Members.
      */
      /***********************************************************************/

      /*
      * @param Members with screen size.
      */
      uint32 m_width;
      uint32 m_height;

      /*
      * @param Members with window size and position.
      */    
      Vector2I m_windowSize;
      Vector2I m_windowPosition;

      /*
      * @param String with the name of the window.
      */
      String m_windowTitle;

      /*
      * @param SFML member with window information.
      */
      Window m_sfmlWindow;
  };
}