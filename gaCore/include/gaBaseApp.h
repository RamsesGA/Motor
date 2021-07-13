#pragma once

#include <tchar.h>
#include <gaVector2I.h>
#include <SFML/Window.hpp>

#include "gaPrerequisitesCore.h"
#include "gaGraphicsApi.h"

using sf::Event;
using sf::Window;
using sf::VideoMode;
using sf::WindowHandle;

namespace gaEngineSDK {

  class SceneGraph;
  class ResourceManager;

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

      /*
      * @brief Load the DLL and initialize the graphic api.
      */
      int32
      initSys();

      void
      handleWindowEvents(Event& windowEvent, const float& deltaTime);

    protected:
      /*
      * @param Create the information for the window.
      */
      void
      createWindow(String windowTitle);

      /*
      * @param Command to call the onResize function.
      */
      void
      resize(int32 width, int32 height);

      /***********************************************************************/
      /*
      * On Methods (virtual).
      */
      /***********************************************************************/

      /*
      * @brief Initialize the camera and all its default values.
      * @param isOGL, Bool to indicate if you are using OGL (this will be removed).
      */
      virtual void
      onInitCamera() { };

      /*
      * @brief Update the constant buffer of the api.
      * @param deltaTime, float for delta time.
      */
      virtual void
      onUpdate(float deltaTime) { };

      /*
      * @brief Save new data, update other and clean old data.
      */
      virtual void
      onRender() { };

      /*
      * @brief Create the data for the api and
      *        confirm that everything exists without problem.
      */
      virtual void
      onCreate() { };

      /*
      * @brief Delete variables to end the program.
      */
      virtual void
      onDestroySystem() { };

      /*
      * @brief Detect keyboard input.
      * @param param, SFML event to know the input pressed.
      */
      virtual void
      onKeyboardDown(sf::Event param, const float& deltaTime) { };

      /*
      * @brief Indicates if the left mouse button is pressed.
      */
      virtual void
      onLeftMouseBtnDown() { };

      /*
      * @brief Indicates whether the left mouse button was lifted.
      */
      virtual void
      onLeftMouseBtnUp() { };

      /*
      * @brief Detects if the mouse is moving on the screen.
      */
      virtual void
      onMouseMove() { };

    protected:
      /***********************************************************************/
      /*
      * Members.
      */
      /***********************************************************************/

      /*
      * @param Members with screen size.
      */
      uint32 m_width = 0;
      uint32 m_height = 0;

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