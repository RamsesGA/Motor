#include "gaBaseApp.h"
#include "gaGraphicsApi.h"

namespace gaEngineSDK {

  int32 
  gaEngineSDK::BaseApp::run() {
    createWindow(m_width, m_height, "GA Window");
    
    //create();
    
    sf::Clock deltaTime;
    float trueDeltaTime = 0.0f;
    
    while (m_sfmlWindow.isOpen()) {
      trueDeltaTime = deltaTime.getElapsedTime().asSeconds();
    
      deltaTime.restart();
    
      //update(trueDeltaTime);
      //render();
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

  void 
  BaseApp::update(float deltaTime) {
    ConstantBuffer1::E meshData;

    meshData.mProjection = m_mainCamera.getProjection();
    meshData.mView = m_mainCamera.getView();

    m_pGraphicApi->updateConstantBuffer
    (&meshData, *m_pConstantBuffer1);

    ConstantBuffer2::E cb;
    cb.mWorld = m_world;
    cb.vMeshColor = m_vMeshColor;

    m_pGraphicApi->updateConstantBuffer(&cb, *m_pConstantBuffer2);
  }

  void 
  BaseApp::render() {
    //Guardamos los render targets
    m_pGraphicApi->setRenderTarget(m_pRenderTargetView, m_pDepthStencil);

    //Guardamos un viewport
    m_pGraphicApi->setViewport(1, m_width, m_height);

    //
    // Clear the back buffer
    //
    m_pGraphicApi->clearYourRenderTargetView(m_pRenderTargetView,
                                             0.0f, 156.0f, 140.0f, 1.6f);
    //
    // Clear the depth buffer to 1.0 (max depth)
    //
    m_pGraphicApi->clearYourDepthStencilView(m_pDepthStencil);

    //Guardamos el input layout
    m_pGraphicApi->setInputLayout(*m_pVertexLayout);
    //Guardamos el vertex buffer
    m_pGraphicApi->setVertexBuffer(*m_pVertexBuffer);
    //Guardamos el index buffer
    m_pGraphicApi->setIndexBuffer(*m_pIndexBuffer);
    //Guardamos la topología
    //m_pGraphicApi->setPrimitiveTopology(PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    m_pGraphicApi->setShaders(*m_pBothShaders);
    m_pGraphicApi->setYourVSConstantBuffers(m_pConstantBuffer1, 0, 1);
    m_pGraphicApi->setYourVSConstantBuffers(m_pConstantBuffer2, 1, 1);
    m_pGraphicApi->setYourPSConstantBuffers(m_pConstantBuffer2, 1, 1);

    //m_model->draw(m_pGraphicApi);

    //
    // Present our back buffer to our front buffer
    //
    m_pGraphicApi->swapChainPresent(0, 0);
  }

  void 
  BaseApp::create() {
    //Mandamos la ventana a la API
    //m_pGraphicApi->initDevice(hWnd);

    initCamera();

    //
    // C R E A T E´s
    //

    //Creamos el render target view
    m_pRenderTargetView = m_pGraphicApi->getDefaultBackBuffer();

    //Creamos el depth stencil view
    m_pDepthStencil = m_pGraphicApi->getDefaultDepthStencil();

    //Creamos el vertex shader y pixel shader
    m_pBothShaders = m_pGraphicApi->createShadersProgram(L"DX_CubeShader.fx", "VS",
                                                         L"DX_CubeShader.fx", "PS");
    if (nullptr == m_pBothShaders) {
      m_pBothShaders = m_pGraphicApi->createShadersProgram(L"OGL_VertexShader.txt", "main",
                                                           L"OGL_PixelShader.txt", "main");
      if (nullptr == m_pBothShaders) {
        exit(1);
      }
      initCamera(true);
    }

    //Creamos el input layout
    m_pVertexLayout = m_pGraphicApi->createInputLayout(*m_pBothShaders);
    if (nullptr == m_pVertexLayout) {
      exit(1);
    }

    //Creamos el vertex buffer
    m_pVertexBuffer = m_pGraphicApi->createVertexBuffer(nullptr, sizeof(Matrices::E));
    if (nullptr == m_pVertexBuffer) {
      exit(1);
    }

    //Creamos el index buffer
    m_pIndexBuffer = m_pGraphicApi->createIndexBuffer(nullptr, sizeof(ViewCB::E));
    if (nullptr == m_pIndexBuffer) {
      exit(1);
    }

    //Creamos los constant buffers para el shader
    m_pConstantBuffer1 = m_pGraphicApi->createConstantBuffer(sizeof(ConstantBuffer1::E));
    if (nullptr == m_pConstantBuffer1) {
      exit(1);
    }

    m_pConstantBuffer2 = m_pGraphicApi->createConstantBuffer(sizeof(ConstantBuffer2::E));
    if (nullptr == m_pConstantBuffer2) {
      exit(1);
    }

    m_model = new Model();
    //g_model->Init("Models/POD/OBJ/POD.obj", m_pGraphicApi);
    //g_model->Init("Models/ugandan/FBX/Knuckles.fbx", m_pGraphicApi);
    //g_model->Init("Models/sonic/FBX/sonic.fbx", m_pGraphicApi);
    //g_model->init("Models/Nier2b/OBJ/Nier2b.obj", m_pGraphicApi);
  }

  void
  BaseApp::destroySystem() {
    //delete m_model;
    delete m_pRenderTargetView;
    delete m_pDepthStencil;
    delete m_pVertexLayout;
    delete m_pBothShaders;
    delete m_pVertexBuffer;
    delete m_pIndexBuffer;
    delete m_pConstantBuffer1;
    delete m_pConstantBuffer2;
    delete m_pGraphicApi;
  }

  void
  BaseApp::initCamera(bool isOGL) {
    //Inicializamos la matriz de identidad
    m_world = 
    {
      1,0,0,0,
      0,1,0,0,
      0,0,1,0,
      0,0,0,1
    };
    
    CameraDescriptor::E mainCamera;
    mainCamera.camLookAt = Vector3(0.0f, 1.0f, 0.0f);
    mainCamera.camEye = Vector3(0.0f, 3.0f, -6.0f);
    mainCamera.camUp = Vector3(0.0f, 1.0f, 0.0f);
    mainCamera.camFar = 1000.0f;
    mainCamera.camNear = 0.01f;
    mainCamera.camFoV = 0.78539816339f;
    mainCamera.camHeight = m_height;
    mainCamera.camWidth = m_width;
    
    m_mainCamera.init(mainCamera, isOGL);
  }
}