#pragma once

#include <tchar.h>
#include <gaVector2I.h>
#include <SFML/Window.hpp>

#include "gaPrerequisitesCore.h"
#include "gaBaseInputs.h"

using sf::Event;
using sf::Window;
using sf::VideoMode;
using sf::WindowHandle;
using sf::Clock;
using sf::Mouse;

namespace gaEngineSDK {
  class SceneGraph;
  class ResourceManager;
  class GraphicsApi;

  class GA_CORE_EXPORT BaseApp
  {
   public:
    /*************************************************************************/
    /*
    * Constructor and destructor.
    */
    /*************************************************************************/
    BaseApp() = default;

    virtual
    ~BaseApp() = default;

    /*************************************************************************/
    /*
    * Methods.
    */
    /*************************************************************************/

    /*
    * @brief Run is the function that starts the
    *      entire creation and initialization process.
    * @param windowTitle, assigning a name.
    * @param sizeX, screen size x.
    * @param sizeY, screen size y.
    * @return Return 0 or 1 to confirm the function.
    */
    int32
    run(String windowTitle, int32 sizeX = 1440, int32 sizeY = 900);

    /*
    * @brief Load the DLL and initialize the graphic api.
    * @return Return 0 or 1 to confirm the function.
    */
    int32
    initSys();

   protected:
    /*
    * @brief Create the information for the window.
    * @param windowTitle, assigning a name..
    */
    void
    createWindow(String windowTitle);

    /*
    * @brief Command to call the onResize function.
    * @param width, screen size x.
    * @param height, screen size y.
    */
    void
    resize(int32 width, int32 height);


    void
    handleWindowEvents(Event& windowEvent, const float& deltaTime);

    /*************************************************************************/
    /*
    * On Methods (virtual).
    */
    /*************************************************************************/

    /*
    * @brief Initialize the camera and all its default values.
    */
    virtual void
    onInitCamera() { };

    /*
    * @brief Update the constant buffer of the api.
    * @param deltaTime, time elapsed since the last frame.
    */
    virtual void
    onUpdate(float deltaTime) { };

    /*
    * @brief Save new data, update other and clean old data.
    */
    virtual void
    onRender() { };

    /*
    * @brief Create the data for the api and confirm that everything exists without problem.
    */
    virtual void
    onCreate() { };

   protected:
    /*************************************************************************/
    /*
    * Members.
    */
    /*************************************************************************/

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
    * @param SFML member with window information.
    */
    Window m_sfmlWindow;
  };
}