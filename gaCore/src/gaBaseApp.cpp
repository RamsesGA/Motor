#include "gaBaseApp.h"
#include "gaGraphicsApi.h"

namespace gaEngineSDK {

  int32 
  gaEngineSDK::BaseApp::run(String windowTitle, int32 sizeX, int32 sizeY) {
    m_width = sizeX;
    m_height = sizeY;

    createWindow(windowTitle);
    
    onInit();

    onCreate();
    
    sf::Clock deltaTime;
    float trueDeltaTime = 0.0f;

    while (m_sfmlWindow.isOpen()) {
      sf::Event event;

      while (m_sfmlWindow.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
          m_sfmlWindow.close();
          break;
        }
        if (event.type == sf::Event::KeyPressed) {
          onKeyboardDown(event);
        }
        if (event.type == sf::Event::MouseButtonPressed) {
          if (sf::Mouse::Left == event.key.code) {
            onLeftMouseBtnDown();
          }
        }
        if (event.type == sf::Event::MouseButtonReleased) {
          if (sf::Mouse::Left == event.key.code) {
            onLeftMouseBtnUp();
          }
        }
        if (event.type == sf::Event::MouseMoved) {
          onMouseMove();
        }
      }

      trueDeltaTime = deltaTime.getElapsedTime().asSeconds();
    
      deltaTime.restart();
    
      onUpdate(trueDeltaTime);
      onRender();
    }

    onDestroySystem();
    
    return 0;
  }

  void 
  BaseApp::createWindow(String windowTitle) {
    m_windowSize.m_x = m_width;
    m_windowSize.m_y = m_height;

    m_windowTitle = windowTitle;

    m_sfmlWindow.create(sf::VideoMode(m_windowSize.m_x, m_windowSize.m_y), m_windowTitle);
  }

  void 
  BaseApp::resize(int32 , int32 ) { }
}