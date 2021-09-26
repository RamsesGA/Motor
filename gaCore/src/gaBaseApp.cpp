#include <Windows.h>

#include "gaBaseApp.h"
#include "gaSceneGraph.h"
#include "gaGraphicsApi.h"
#include "gaBaseRenderer.h"
#include "gaBaseInterface.h"
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
    float trueDeltaTime = 0.0f;
    Clock deltaTime;

    /*
    * Init Renderer
    */
    auto myRenderer = g_baseRenderer().instancePtr();
    myRenderer->init(m_width, m_height);

    /*
    * Init ImGui
    */
    auto myInterface = g_baseInterface().instancePtr();
    myInterface->init(m_width, m_height, m_sfmlWindow.getSystemHandle());

    /*
    * Init inputs
    */
    auto myInputs = g_baseInputs().instancePtr();
    myInputs->init(m_width, m_height, m_sfmlWindow.getSystemHandle());

    /*
    * W I N D O W
    */
    auto myGraphicsApi = g_graphicApi().instancePtr();

    //We save a viewport.
    myGraphicsApi->setViewports(m_width, m_height);

    while (m_sfmlWindow.isOpen()) {
      Event event;

      trueDeltaTime = deltaTime.getElapsedTime().asSeconds();

      MSG msg;
      while (PeekMessageW(&msg, m_sfmlWindow.getSystemHandle(), 0, 0, PM_REMOVE)) {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
        
        myInputs->handleMsg(msg);
      }

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

      myInputs->update(trueDeltaTime);
      myInterface->update(trueDeltaTime);
      myRenderer->update(trueDeltaTime);
      onUpdate(trueDeltaTime);

      myInterface->render();
      myRenderer->render();
      onRender();
    }

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
    * B A S E
    * I N T E R F A C E
    */
    hInstance = LoadLibraryExA("gaInterface_d.dll", nullptr, LOAD_WITH_ALTERED_SEARCH_PATH);

    //In case of error
    if (!(hInstance)) {
      return -1;
    }

    using fnBI = BaseInterface * (*)();
    fnBI baseInter = reinterpret_cast<fnBI>(GetProcAddress(hInstance, "createNewInterface"));

    //In case of error
    if (!(baseInter)) {
      return -1;
    }

    BaseInterface::startUp();
    BaseInterface* newBI = baseInter();
    g_baseInterface().setObject(newBI);

    /*
    * B A S E
    * I N P U T S
    */
    hInstance = LoadLibraryExA("gaInputs_d.dll", nullptr, LOAD_WITH_ALTERED_SEARCH_PATH);

    //In case of error
    if (!(hInstance)) {
      return -1;
    }

    using fnBInputs = BaseInputs * (*)();
    fnBInputs baseInputs = reinterpret_cast<fnBInputs>(GetProcAddress(hInstance, "newInputs"));

    //In case of error
    if (!(baseInputs)) {
      return -1;
    }

    BaseInputs::startUp();
    BaseInputs* newBInputs = baseInputs();
    g_baseInputs().setObject(newBInputs);

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
    auto myInterface = g_baseInterface().instancePtr();

    switch (windowEvent.type) {
      case Event::MouseButtonPressed:
        if ((Mouse::Left == windowEvent.key.code) && (!myInterface->m_touchingImGui)) {
          myRenderer->onLeftMouseBtnDown();
        }
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