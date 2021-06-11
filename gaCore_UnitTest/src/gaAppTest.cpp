#include <Windows.h>
#include <iostream>
#include <gaModels.h>
#include <gaResourceManager.h>
#include <gaRenderModels.h>

#include "gaAppTest.h"

int32
AppTest::onInit() {
  HINSTANCE hInstance = LoadLibraryExA("gaDirectX_d.dll", nullptr,
                                       LOAD_WITH_ALTERED_SEARCH_PATH);
  //HINSTANCE hInstance = LoadLibraryExA("gaOpenGL_d.dll", nullptr,
  //                                     LOAD_WITH_ALTERED_SEARCH_PATH);

  //In case of error
  if (!(hInstance)) {
    return -1;
  }

  using fnProt = GraphicsApi * (*)();

  fnProt graphicsApiFunc = reinterpret_cast<fnProt>(GetProcAddress
                                                   (hInstance, "createGraphicApi"));

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
AppTest::onInitCamera() {
  //Inicializamos la matriz de identidad
  m_world.identity();

  CameraDescriptor::E mainCamera;
  mainCamera.camLookAt = Vector3(0.0f, 1.0f, 0.0f);
  mainCamera.camEye = Vector3(0.0f, 0.0f, -700.0f);
  mainCamera.camUp = Vector3(0.0f, 1.0f, 0.0f);
  mainCamera.camFar = 300000.0f;
  mainCamera.camNear = 0.01f;
  mainCamera.camFoV = Math::FOV;
  mainCamera.camHeight = m_height;
  mainCamera.camWidth = m_width;

  m_mainCamera.init(mainCamera);
}

void
AppTest::onUpdate(float deltaTime) {
  auto myGraphicsApi = g_graphicApi().instancePtr();

  ConstantBuffer1::E meshData;
  meshData.mProjection = myGraphicsApi->matrix4x4Context(m_mainCamera.getProjection());
  meshData.mView = myGraphicsApi->matrix4x4Context(m_mainCamera.getView());

  ConstantBuffer2::E cb;
  cb.mWorld = m_world;
  cb.vMeshColor = m_vMeshColor;

  try {
    myGraphicsApi->updateConstantBuffer(&meshData, *m_pConstantBuffer1);
    myGraphicsApi->updateConstantBuffer(&cb, *m_pConstantBuffer2);

    m_renderModel->update(*m_resourceManager, deltaTime);
  }
  catch (exception* e) {
    std::cout << "- - > " << e->what() << '\n';
    exit(1);
  }
}

void
AppTest::onRender() {

  try {
    auto myGraphicsApi = g_graphicApi().instancePtr();

    //Guardamos los render targets
    myGraphicsApi->setRenderTarget(m_pRenderTargetView, m_pDepthStencil);

    //Guardamos un viewport
    myGraphicsApi->setViewport(1, m_width, m_height);

    //Clear the back buffer
    myGraphicsApi->clearYourRenderTargetView(m_pRenderTargetView, (87.0f / 255.0f),
                                            (35.0f / 255.0f), (100.0f / 255.0f), 
                                             255.0f);

    // Clear the depth buffer to 1.0 (max depth)
    myGraphicsApi->clearYourDepthStencilView(m_pDepthStencil);

    //Guardamos el input layout
    myGraphicsApi->setInputLayout(*m_pVertexLayout);

    //Guardamos el vertex buffer
    myGraphicsApi->setVertexBuffer(*m_mesh->m_pVertexBuffer);

    //Guardamos el index buffer
    myGraphicsApi->setIndexBuffer(*m_mesh->m_pIndexBuffer);

    //Guardamos la topología
    myGraphicsApi->setPrimitiveTopology(PRIMITIVE_TOPOLOGY::E::kTriangleList);

    myGraphicsApi->setShaders(*m_pBothShaders);
    myGraphicsApi->setYourVSConstantBuffers(m_pConstantBuffer1, 0, 1);
    myGraphicsApi->setYourVSConstantBuffers(m_pConstantBuffer2, 1, 1);
    myGraphicsApi->setYourPSConstantBuffers(m_pConstantBuffer2, 1, 1);
    myGraphicsApi->setYourVSConstantBuffers(m_tempBufferBones.get(), 2, 1);
    //myGraphicsApi->setYourPSConstantBuffers(m_pConstBufferBones, 2, 1);

    m_renderModel->drawModel(*m_resourceManager, m_tempBufferBones);
  }
  catch (exception* e) {
    std::cout << "- - > " << e->what() << '\n';
    exit(1);
  }
  
  // Present our back buffer to our front buffer
  g_graphicApi().swapChainPresent(0, 0);
}

void
AppTest::onCreate() {

  auto myGraphicsApi = g_graphicApi().instancePtr();

  onInitCamera();

  //
  // C R E A T E´s
  //

  //Creamos el render target view
  m_pRenderTargetView = myGraphicsApi->getDefaultBackBuffer();

  //Creamos el depth stencil view
  m_pDepthStencil = myGraphicsApi->getDefaultDepthStencil();

  try {
    m_renderModel = new RenderModels();
    m_mesh = new Mesh();
    m_resourceManager = new ResourceManager();

    //Creamos el vertex shader y pixel shader.
    //DX_animation
    //DX_color
    m_pBothShaders = myGraphicsApi->createShadersProgram(L"data/shaders/DX_animation.fx",
                                                         "VS",
                                                         L"data/shaders/DX_animation.fx",
                                                         "PS");
    //Creamos el vertex shader y pixel shader.
    //m_pBothShaders = myGraphicsApi->createShadersProgram(L"data/shaders/OGL_VertexShader.txt",
    //                                                     "main",
    //                                                     L"data/shaders/OGL_PixelShader.txt",
    //                                                     "main");

    //Creamos el input layout 
    m_pVertexLayout = myGraphicsApi->createInputLayout(*m_pBothShaders);

    //Creamos el vertex buffer
    m_mesh->m_pVertexBuffer = myGraphicsApi->createVertexBuffer(nullptr, sizeof(Matrices::E));

    //Creamos el index buffer 
    m_mesh->m_pIndexBuffer = myGraphicsApi->createIndexBuffer(nullptr, sizeof(ViewCB::E));

    //Creamos los constant buffers para el shader
    m_pConstantBuffer1 = myGraphicsApi->createConstantBuffer(sizeof(ConstantBuffer1::E));
    m_pConstantBuffer2 = myGraphicsApi->createConstantBuffer(sizeof(ConstantBuffer2::E));
    m_pConstBufferBones = myGraphicsApi->createConstantBuffer(sizeof(ConstBuffBonesTransform::E));
    m_tempBufferBones.reset(m_pConstBufferBones);
  }
  catch (exception* e) {
    std::cout << "- - > " << e->what() << '\n';
    exit(1);
  }

  try {
    //m_resourceManager->initLoadModel("data/models/2B/2B.obj");
    //m_resourceManager->initLoadModel("data/models/pod/POD.obj");
    m_resourceManager->initLoadModel("data/models/spartan/Spartan.fbx");
    //m_resourceManager->initLoadModel("data/models/ugandan/Knuckles.fbx");
    //m_resourceManager->initLoadModel("data/models/grimoires/grimoires.fbx");

    //Esto solo es para guardar los huesos y las animaciones
    //En caso de que no, evitamos un error
    if (0 != m_resourceManager->getMeshes().size()) {
      m_renderModel->setMeshBones(*m_resourceManager);

      if (0 != m_resourceManager->getModel()->m_vAnimationData.size()) {
        m_renderModel->m_currentAnimation = m_resourceManager->getModel()->m_vAnimationData[0];
      }
    }
  }
  catch (exception* e) {
    std::cout << "- - > " << e->what() << '\n';
  }
}

void
AppTest::onDestroySystem() {
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

  auto myGraphicsApi = g_graphicApi().instancePtr();

  ConstantBuffer1::E cb;
  cb.mView = myGraphicsApi->matrix4x4Context(m_mainCamera.getView());

  myGraphicsApi->updateConstantBuffer(&cb, *m_pConstantBuffer1);
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
    m_mainCamera.setOriginalMousePos
    (
      m_mainCamera.getOriginalMousePos().m_x,
      m_mainCamera.getOriginalMousePos().m_y
    );

    m_mainCamera.mouseRotation();

    auto myGraphicsApi = g_graphicApi().instancePtr();

    ConstantBuffer1::E cb;
    cb.mView = myGraphicsApi->matrix4x4Context(m_mainCamera.getView());

    myGraphicsApi->updateConstantBuffer(&cb, *m_pConstantBuffer1);
  }
}