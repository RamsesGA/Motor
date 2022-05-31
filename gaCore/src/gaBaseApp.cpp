#include <Windows.h>

#include "gaBaseApp.h"
#include "gaSceneGraph.h"
#include "gaGraphicsApi.h"
#include "gaBaseRenderer.h"
#include "gaBaseInterface.h"
#include "gaBasePhysics.h"
#include "gaBaseOmniConnect.h"
#include "gaResourceManager.h"

namespace gaEngineSDK {
  int32 
  BaseApp::run(String windowTitle, int32 sizeX, int32 sizeY) {
    m_width = sizeX;
    m_height = sizeY;

    createWindow(windowTitle);
    
    initSys();

    onCreate();

    /*
    * Init Renderer D3D11
    */
    auto myRenderer = g_baseRenderer().instancePtr();
    myRenderer->init(m_width, m_height);

    /*
    * Init Renderer D3D12
    */
    //auto myD3D12Renderer = g_baseRenderer().instancePtr();
    //myD3D12Renderer->init(m_width, m_height);

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

    bool shoudClose = false;
    float trueDeltaTime = 0.0f;
    Clock deltaTime;

    auto myOmniverse = g_baseOmniConnect().instancePtr();

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

      /*
      * O M N I V E R S E
      * C R E A T E
      * S T A G E
      */
      if (myInterface->m_startOmniverse) {
        myInterface->m_startOmniverse = false;

        if (!(m_initOmni)) {
          omniverseTest();
          m_initOmni = true;
        }

        myInterface->m_isRunningOmniverse = true;
      }
      
      if (myInterface->m_isRunningOmniverse) {
        myOmniverse->updateOmniverseToGa();
      }

      myInputs->update(trueDeltaTime);
      myRenderer->update(trueDeltaTime);
      //myD3D12Renderer->update(trueDeltaTime);
      myInterface->update(trueDeltaTime);
      onUpdate(trueDeltaTime);

      myRenderer->render();
      //myD3D12Renderer->render();
      myInterface->render();
      onRender();
    }

    return 0;
  }

  int32
  BaseApp::initSys() {
    /*
    * D 3 D 1 1
    * 
    * typeFn = 0
    */
    if (!(loadDlls("gaDirectX_d.dll", "createGraphicApi", 0))) {
      return -1;
    }
        
    /*
    * B A S E
    * R E N D E R E R
    * D E F E R R E D
    * R E N D E R I N G
    * A P I
    * 
    * typeFn = 1
    */
    if (!(loadDlls("gaDeferredRendering_d.dll", "createBaseRenderer", 1))) {
      return -1;
    }

    /*
    * B A S E
    * I N T E R F A C E
    * 
    * typeFn = 2
    */
    if (!(loadDlls("gaInterface_d.dll", "createNewInterface", 2))) {
      return -1;
    }

    /*
    * B A S E
    * I N P U T S
    * 
    * typeFn = 3
    */
    if (!(loadDlls("gaInputs_d.dll", "newInputs", 3))) {
      return -1;
    }
    
    /*
    * B A S E
    * O M N I V E R S E
    * 
    * typeFn = 4
    */
    if (!(loadDlls("gaOmniverseConnect_d.dll", "createOmniConnect", 4))) {
      return -1;
    }

    /*
    * P H Y S I C S
    *
    * typeFn = 5
    */
    //if (!(loadDlls("gaPhysics_d.dll", "newPhysics"))) {
    //  return -1;
    //}
    
    /*
    * M O D U L E
    * I N I T I A L I Z A T I O N
    */
    SceneGraph::startUp();
    ResourceManager::startUp();

    return 0;
  }

  bool
  BaseApp::loadDlls(String nameDll, String nameDllFunc, uint32 typeFn) {
    HINSTANCE hInstance = LoadLibraryExA(nameDll.c_str(),
                                         nullptr,
                                         LOAD_WITH_ALTERED_SEARCH_PATH);
    //In case of error
    if (!(hInstance)) {
      return false;
    }

    /*
    * G R A P H I C S
    * A P I
    */
    if (0 == typeFn) {
      using fn0 = GraphicsApi * (*)();
      fn0 apiFunc = reinterpret_cast<fn0>(GetProcAddress(hInstance, 
                                                         nameDllFunc.c_str()));
      //In case of error
      if (!(apiFunc)) {
        return false;
      }

      GraphicsApi::startUp();
      GraphicsApi* graphicApi = apiFunc();
      g_graphicApi().setObject(graphicApi);
    }

    /*
    * D E F E R R E D
    * R E N D E R I N G
    */
    if (1 == typeFn) {
      using fn1 = BaseRenderer * (*)();
      fn1 baseRendApiFunc = reinterpret_cast<fn1>(GetProcAddress(hInstance,
                                                                 nameDllFunc.c_str()));
      //In case of error
      if (!(baseRendApiFunc)) {
        return false;
      }
      
      BaseRenderer::startUp();
      BaseRenderer* newBR = baseRendApiFunc();
      g_baseRenderer().setObject(newBR);
    }

    /*
    * I N T E R F A C E
    */
    if (2 == typeFn) {
      using fn2 = BaseInterface * (*)();
      fn2 baseInter = reinterpret_cast<fn2>(GetProcAddress(hInstance, 
                                                           nameDllFunc.c_str()));
      //In case of error
      if (!(baseInter)) {
        return false;
      }

      BaseInterface::startUp();
      BaseInterface* newBI = baseInter();
      g_baseInterface().setObject(newBI);
    }

    /*
    * I N P U T S
    */
    if (3 == typeFn) {
      using fn3 = BaseInputs * (*)();
      fn3 baseInputs = reinterpret_cast<fn3>(GetProcAddress(hInstance, 
                                                            nameDllFunc.c_str()));
      //In case of error
      if (!(baseInputs)) {
        return false;
      }

      BaseInputs::startUp();
      BaseInputs* newBInputs = baseInputs();
      g_baseInputs().setObject(newBInputs);
    }

    /*
    * O M N I V E R S E
    */
    if (4 == typeFn) {
      using fn4 = BaseOmniConnect * (*)();
      fn4 baseOmni = reinterpret_cast<fn4>(GetProcAddress(hInstance,
                                                          nameDllFunc.c_str()));
      //In case of error
      if (!(baseOmni)) {
        return false;
      }

      BaseOmniConnect::startUp();
      BaseOmniConnect* newBOmni = baseOmni();
      g_baseOmniConnect().setObject(newBOmni);
    }

    /*
    * P H Y S I C S
    */
    if (5 == typeFn) {
      using fn5 = BasePhysics * (*)();
      fn5 basePhys = reinterpret_cast<fn5>(GetProcAddress(hInstance, 
                                                          nameDllFunc.c_str()));
      //In case of error
      if (!(basePhys)) {
        return false;
      }

      BasePhysics::startUp();
      BasePhysics* newBPhys = basePhys();
      g_basePhysics().setObject(newBPhys);
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

  void 
  BaseApp::handleWindowEvents(Event& windowEvent, const float& deltaTime) {
    auto myRenderer = g_baseRenderer().instancePtr();
    auto myInterface = g_baseInterface().instancePtr();

    switch (windowEvent.type) {
      case Event::MouseButtonPressed:
        if ((Mouse::Left == windowEvent.key.code) && (!myInterface->m_touchingImGui)) {
          myRenderer->onLeftMouseBtnDown();
        }

        myInterface->mouseButtonDown(windowEvent.mouseButton.button);
        break;

      case Event::MouseButtonReleased:
        myInterface->mouseButtonReleased(windowEvent.mouseButton.button);
        break;

      case Event::TextEntered:
        myInterface->textEnter(windowEvent.text.unicode);
        break;

      default:
        break;
    }
  }

  void
  BaseApp::omniverseTest() {
    auto myOmniverse = g_baseOmniConnect().instancePtr();
    String destinationPath = "http://localhost:8080/omniverse://127.0.0.1/Users/gaEngine/";

    if (!(myOmniverse->startOmniverse())) {
      std::cout << "\nError, can not start Omniverse\n";
      exit(1);
    }

    //Print the username for the server
    myOmniverse->printConnectedUsername(destinationPath);

    //Create the USD model in Omniverse.
    const String stageUrl = myOmniverse->createOmniverseScene(destinationPath, "test");

    if (stageUrl.empty()) {
      std::cout << "\nError, can not create Omniverse scene\n";
      exit(1);
    }

    myOmniverse->openNewUSDFile("http://localhost:8080/omniverse://127.0.0.1/Users/gaEngine/test.usd");

    myOmniverse->saveSceneGraphToUSD();
  }
}