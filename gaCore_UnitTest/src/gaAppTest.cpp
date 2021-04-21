#include "gaAppTest.h"
#include <Windows.h>
#include <iostream>

#define D3D11
//#define OGL

int32
AppTest::onInit() {
#ifdef D3D11
  HINSTANCE hInstance = LoadLibraryExA("gaDirectX_d.dll", nullptr,
                                       LOAD_WITH_ALTERED_SEARCH_PATH);
#elif defined OGL
  HINSTANCE hInstance = LoadLibraryExA("gaOpenGL_d.dll", nullptr,
                                       LOAD_WITH_ALTERED_SEARCH_PATH);
#endif

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

  //Mandamos la ventana a la API
  g_graphicApi().initDevice(m_sfmlWindow.getSystemHandle());

  //FreeLibrary(hInstance);

  return 0;
}

void
AppTest::onInitCamera(bool isOGL) {
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

  EXPECT_NEAR(m_mainCamera.getProjection().matrixData3(0).m_x, 1.5000f, Math::BIG_NUMBER);
  EXPECT_NEAR(m_mainCamera.getProjection().matrixData3(1).m_y, 2.4142f, Math::BIG_NUMBER);
  EXPECT_NEAR(m_mainCamera.getProjection().matrixData3(2).m_z, 1.0000f, Math::BIG_NUMBER);
}

void
AppTest::onUpdate(float ) {
  ConstantBuffer1::E meshData;
  meshData.mProjection = m_mainCamera.getProjection();
  meshData.mView = m_mainCamera.getView();

  ConstantBuffer2::E cb;
  cb.mWorld = m_world;
  cb.vMeshColor = m_vMeshColor;

  try {
    g_graphicApi().updateConstantBuffer(&meshData, *m_pConstantBuffer1);
    g_graphicApi().updateConstantBuffer(&cb, *m_pConstantBuffer2);
  }
  catch (exception* e) {
    std::cout << "- - > " << e->what() << '\n';
  }
}

void
AppTest::onRender() {

  try {
    //Guardamos los render targets
    g_graphicApi().setRenderTarget(m_pRenderTargetView, m_pDepthStencil);

    //Guardamos un viewport
    g_graphicApi().setViewport(1, m_width, m_height);

    //Clear the back buffer
    g_graphicApi().clearYourRenderTargetView(m_pRenderTargetView, (87.0f / 255.0f),
                                            (35.0f / 255.0f), (100.0f / 255.0f), 
                                             255.0f);

    // Clear the depth buffer to 1.0 (max depth)
    g_graphicApi().clearYourDepthStencilView(m_pDepthStencil);

    //Guardamos el input layout
    g_graphicApi().setInputLayout(*m_pVertexLayout);

    //Guardamos el vertex buffer
    g_graphicApi().setVertexBuffer(*m_pVertexBuffer);

    //Guardamos el index buffer
    g_graphicApi().setIndexBuffer(*m_pIndexBuffer);

    //Guardamos la topología
    g_graphicApi().setPrimitiveTopology(PRIMITIVE_TOPOLOGY::E::kTriangleList);

    g_graphicApi().setShaders(*m_pBothShaders);
    g_graphicApi().setYourVSConstantBuffers(m_pConstantBuffer1, 0, 1);
    g_graphicApi().setYourVSConstantBuffers(m_pConstantBuffer2, 1, 1);
    g_graphicApi().setYourPSConstantBuffers(m_pConstantBuffer2, 1, 1);

    m_model->draw(&g_graphicApi());
  }
  catch (exception* e) {
    std::cout << "- - > " << e->what() << '\n';
  }
  
  // Present our back buffer to our front buffer
  g_graphicApi().swapChainPresent(0, 0);
}

void
AppTest::onCreate() {

  onInitCamera();

  //
  // C R E A T E´s
  //

  //Creamos el render target view
  m_pRenderTargetView = g_graphicApi().getDefaultBackBuffer();

  //Creamos el depth stencil view
  m_pDepthStencil = g_graphicApi().getDefaultDepthStencil();

  try {
#ifdef D3D11
    //Creamos el vertex shader y pixel shader.
    m_pBothShaders = g_graphicApi().createShadersProgram(L"data/shaders/DX_CubeShader.fx", 
                                                         "VS",
                                                         L"data/shaders/DX_CubeShader.fx", 
                                                         "PS");
#elif defined OGL
    //Creamos el vertex shader y pixel shader.
    m_pBothShaders = g_graphicApi().createShadersProgram(L"data/shaders/OGL_VertexShader.txt",
                                                         "main",
                                                         L"data/shaders/OGL_PixelShader.txt",
                                                         "main");
    onInitCamera(true);
#endif
    //Creamos el input layout 
    m_pVertexLayout = g_graphicApi().createInputLayout(*m_pBothShaders);

    //Creamos el vertex buffer
    m_pVertexBuffer = g_graphicApi().createVertexBuffer(nullptr, sizeof(Matrices::E));

    //Creamos el index buffer 
    m_pIndexBuffer = g_graphicApi().createIndexBuffer(nullptr, sizeof(ViewCB::E));

    //Creamos los constant buffers para el shader
    m_pConstantBuffer1 = g_graphicApi().createConstantBuffer(sizeof(ConstantBuffer1::E));
    m_pConstantBuffer2 = g_graphicApi().createConstantBuffer(sizeof(ConstantBuffer2::E));
  }
  catch (exception* e) {
    std::cout << "- - > " << e->what() << '\n';
  }

  try {
    m_model = new Model();

    //m_model->init("data/models/POD/OBJ/POD.obj", &g_graphicApi());
    //m_model->init("data/models/sonic/FBX/sonic.fbx", &g_graphicApi());
    m_model->init("data/models/Nier2b/OBJ/Nier2b.obj", &g_graphicApi());
    //m_model->init("data/models/ugandan/FBX/Knuckles.fbx", &g_graphicApi());
  }
  catch (exception* e) {
    std::cout << "- - > " << e->what() << '\n';
  }
}

void
AppTest::onDestroySystem() {
  //delete m_model;
  //delete m_pRenderTargetView;
  //delete m_pDepthStencil;
  //delete m_pVertexLayout;
  //delete m_pBothShaders;
  //delete m_pVertexBuffer;
  //delete m_pIndexBuffer;
  //delete m_pConstantBuffer1;
  //delete m_pConstantBuffer2;
}

void
AppTest::onKeyboardDown(sf::Event param) {
  m_mainCamera.inputDetection(param);

  ConstantBuffer1::E cb;
  cb.mView = m_mainCamera.getView();

  g_graphicApi().updateConstantBuffer(&cb, *m_pConstantBuffer1);
}

void
AppTest::onLeftMouseBtnDown() {
  sf::Vector2i position = sf::Mouse::getPosition();

  m_mainCamera.setOriginalMousePos(position.x, position.y);
  m_mainCamera.setClickPressed(true);
}

void
AppTest::onLeftMouseBtnUp() {
  m_mainCamera.setClickPressed(false);
}

void
AppTest::onMouseMove() {
  if (m_mainCamera.getClickPressed()) {
    m_mainCamera.setOriginalMousePos(m_mainCamera.getOriginalMousePos().m_x,
      m_mainCamera.getOriginalMousePos().m_y);
    m_mainCamera.mouseRotation();

    ConstantBuffer1::E cb;
    cb.mView = m_mainCamera.getView();

    g_graphicApi().updateConstantBuffer(&cb, *m_pConstantBuffer1);
  }
}