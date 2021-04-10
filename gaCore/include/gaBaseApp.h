#pragma once

#include <tchar.h>
#include <SFML/Window.hpp>
#include <gaVector2I.h>
#include <gaMatrix4x4.h>
#include <gaVector4.h>

#include "gaPrerequisitesCore.h"
#include "gaGraphicsApi.h"
#include "gaCamera.h"
#include "gaModels.h"

using sf::Window;
using sf::VideoMode;
using sf::WindowHandle;

namespace gaEngineSDK {
  /***************************************************************************/
  /**
  * Structs
  */
  /***************************************************************************/
  namespace ConstantBuffer1 {
    struct E {
      Matrix4x4 mView;
      Matrix4x4 mProjection;
    };
  }

  namespace ConstantBuffer2 {
    struct E {
      Matrix4x4 mWorld;
      Vector4 vMeshColor;
    };
  }

  namespace Matrices {
    struct E{
      Matrix4x4 World;
      Matrix4x4 Projection;
      Vector4   Color;
    };
  }

  namespace ViewCB {
    struct E{
      Matrix4x4 View;
    };
  }
  


  class GA_CORE_EXPORT BaseApp
  {
    public:
      BaseApp() = default;

      virtual 
      ~BaseApp() = default;

      int32
      run();

    protected:
      void
      createWindow(int32 sizeX, int32 sizeY, String windowTitle);

      virtual void
      init(){};

      virtual void
      onInit() {};

      virtual void
      update(float deltaTime);

      virtual void
      onUpdate(float deltaTime) {};

      virtual void
      render();

      virtual void
      onRender() {};

      virtual void
      create();

      virtual void
      onCreate() {};

      virtual void
      resize(int32 width, int32 height) {};

      virtual void
      destroySystem();

      virtual void
      initCamera(bool isOGL = false);

    protected:

      //Tamaño de pantalla
      const unsigned int m_width = 1000;
      const unsigned int m_height = 800;

      Vector2I m_windowSize;
      Vector2I m_windowPosition;

      Vector4 m_vMeshColor = (0.7f, 0.7f, 0.7f, 1.0f);

      String m_windowTitle;

      Matrix4x4 m_world;

      Camera m_mainCamera;
      Model* m_model = nullptr;

      //GraphicsApi* g_pGraphicApi = new GraphicApiDX();
      //GraphicsApi* g_pGraphicApi = new GraphicApiOGL();
      GraphicsApi* m_pGraphicApi;

      Textures* m_pRenderTargetView = nullptr;
      Textures* m_pDepthStencil = nullptr;

      InputLayout* m_pVertexLayout = nullptr;

      Shaders* m_pBothShaders = nullptr;

      VertexBuffer* m_pVertexBuffer = nullptr;

      IndexBuffer* m_pIndexBuffer = nullptr;

      ConstantBuffer* m_pConstantBuffer1 = nullptr;
      ConstantBuffer* m_pConstantBuffer2 = nullptr;

      Window m_sfmlWindow;
  };
}