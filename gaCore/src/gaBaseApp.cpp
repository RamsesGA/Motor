#include "gaBaseApp.h"
#include "gaGraphicsApi.h"

namespace gaEngineSDK {

  int32 
  gaEngineSDK::BaseApp::run() {
    createWindow(900, 900, "GA Window");
    
    create();
    
    sf::Clock deltaTime;
    float trueDeltaTime = 0.0f;
    
    while (m_sfmlWindow.isOpen()) {
      trueDeltaTime = deltaTime.getElapsedTime().asSeconds();
    
      deltaTime.restart();
    
      update(trueDeltaTime);
      render();
    }
    
    destroySystem();
    
    return 0;
  }

  void 
  BaseApp::createWindow(int32 sizeX, int32 sizeY, String windowTitle) {
    m_windowSize.m_x = sizeX;
    m_windowSize.m_y = sizeY;

    m_windowTitle = windowTitle;

    m_sfmlWindow.create(sf::VideoMode(m_windowSize.m_x, m_windowSize.m_y), m_windowTitle);
  }


}