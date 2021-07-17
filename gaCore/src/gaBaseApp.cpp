#include "gaBaseApp.h"
#include "gaSceneGraph.h"

namespace gaEngineSDK {

  int32 
  gaEngineSDK::BaseApp::run(String windowTitle, int32 sizeX, int32 sizeY) {
    m_width = sizeX;
    m_height = sizeY;

    createWindow(windowTitle);
    
    initSys();

    onCreate();
    
    bool shoudClose = false;
    sf::Clock deltaTime;
    float trueDeltaTime = 0.0f;

    while (m_sfmlWindow.isOpen()) {
      Event event;

      trueDeltaTime = deltaTime.getElapsedTime().asSeconds();

      while (m_sfmlWindow.pollEvent(event)) {
        if (Event::Closed == event.type) {
          m_sfmlWindow.close();
          shoudClose = true;
          break;
        }

        handleWindowEvents(event, trueDeltaTime);
      }
      if (shoudClose) {
        break;
      }

      deltaTime.restart();
    
      onUpdate(trueDeltaTime);
      onRender();
    }

    onDestroySystem();
    
    return 0;
  }

  int32
  gaEngineSDK::BaseApp::initSys() {
    HINSTANCE hInstance = LoadLibraryExA("gaDirectX_d.dll", nullptr,
                                         LOAD_WITH_ALTERED_SEARCH_PATH);
    //HINSTANCE hInstance = LoadLibraryExA("gaOpenGL_d.dll", nullptr,
    //                                     LOAD_WITH_ALTERED_SEARCH_PATH);

    //In case of error
    if (!(hInstance)) {
      return -1;
    }

    using fnProt = GraphicsApi * (*)();

    fnProt graphicsApiFunc = reinterpret_cast<fnProt>
                                              (GetProcAddress(hInstance, "createGraphicApi"));

    //In case of error
    if (!(graphicsApiFunc)) {
      return -1;
    }

    GraphicsApi::startUp();
    GraphicsApi* graphicApi = graphicsApiFunc();
    g_graphicApi().setObject(graphicApi);

    //Module initialization
    SceneGraph::startUp();

    return 0;
  }

  void 
  BaseApp::handleWindowEvents(Event& windowEvent, const float& deltaTime) {
    switch (windowEvent.type) {
      case Event::KeyPressed:
        onKeyboardDown(windowEvent, deltaTime);
        break;

      case Event::MouseButtonPressed:
        if (sf::Mouse::Left == windowEvent.key.code) {
          onLeftMouseBtnDown();
        }
        break;

      case Event::MouseButtonReleased:
        if (sf::Mouse::Left == windowEvent.key.code) {
          onLeftMouseBtnUp();
        }
        break;

      case Event::MouseMoved:
        onMouseMove();
        break;

      default:
        break;
    }
  }

  void 
  BaseApp::createWindow(String windowTitle) {
    m_windowSize.x = m_width;
    m_windowSize.y = m_height;

    m_sfmlWindow.create(sf::VideoMode(m_windowSize.x, m_windowSize.y), windowTitle);
  }

  void 
  BaseApp::resize(int32 , int32 ) { }
}