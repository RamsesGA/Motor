#include "gaBaseApp.h"
#include "gaSceneGraph.h"
#include "gaGraphicsApi.h"
#include "gaBaseRenderer.h"
#include "gaResourceManager.h"

namespace gaEngineSDK {
  int32 
  BaseApp::run(String windowTitle, int32 sizeX, int32 sizeY) {
    m_width = sizeX;
    m_height = sizeY;

    createWindow(windowTitle);
    
    initSys();

    onCreate();
    
    bool shoudClose = false;
    Clock deltaTime;
    float trueDeltaTime = 0.0f;

    auto myRenderer = g_baseRenderer().instancePtr();
    myRenderer->init(m_width, m_height);

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
    
      myRenderer->update(trueDeltaTime);
      onUpdate(trueDeltaTime);
      myRenderer->render();
      onRender();
    }

    onDestroySystem();
    
    return 0;
  }

  int32
  BaseApp::initSys() {
    /*
    * G R A P H I C S
    * A P I
    */
    HINSTANCE hInstance = LoadLibraryExA("gaDirectX_d.dll",
                                         nullptr,
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

    /*
    * B A S E
    * R E N D E R E R
    * D E F E R R E D
    * R E N D E R I N G
    * A P I
    */
    hInstance = LoadLibraryExA("gaDeferredRendering_d.dll",
                               nullptr,
                               LOAD_WITH_ALTERED_SEARCH_PATH);
    //In case of error
    if (!(hInstance)) {
      return -1;
    }

    using fnBR = BaseRenderer * (*)();
    fnBR baseRendApiFunc = reinterpret_cast<fnBR>(GetProcAddress
                                                  (hInstance, "createBaseRenderer"));
    //In case of error
    if (!(baseRendApiFunc)) {
      return -1;
    }

    BaseRenderer::startUp();
    BaseRenderer* newBR = baseRendApiFunc();
    g_baseRenderer().setObject(newBR);

    /*
    * M O D U L E
    * I N I T I A L I Z A T I O N
    */
    SceneGraph::startUp();
    ResourceManager::startUp();

    return 0;
  }

  void 
  BaseApp::handleWindowEvents(Event& windowEvent, const float& deltaTime) {
    auto myRenderer = g_baseRenderer().instancePtr();

    switch (windowEvent.type) {
      case Event::KeyPressed:
        myRenderer->onKeyboardDown(windowEvent, deltaTime);
        break;

      case Event::MouseButtonPressed:
        if (Mouse::Left == windowEvent.key.code) {
          myRenderer->onLeftMouseBtnDown();
        }
        break;

      case Event::MouseButtonReleased:
        if (Mouse::Left == windowEvent.key.code) {
          myRenderer->onLeftMouseBtnUp();
        }
        break;

      case Event::MouseMoved:
        myRenderer->onMouseMove(deltaTime);
        break;

      default:
        break;
    }
  }

  void 
  BaseApp::createWindow(String windowTitle) {
    m_windowSize.x = m_width;
    m_windowSize.y = m_height;

    m_sfmlWindow.create(VideoMode(m_windowSize.x, m_windowSize.y), windowTitle);
  }

  void 
  BaseApp::resize(int32 , int32 ) { }
}