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

  m_mainCamera.setLookAt(Vector3(0.0f, 1.0f, 0.0f));
  m_mainCamera.setEye(Vector3(0.0f, 0.0f, -700.0f));
  m_mainCamera.setUp(Vector3(0.0f, 1.0f, 0.0f));
  m_mainCamera.setFar();
  m_mainCamera.setNear();
  m_mainCamera.setFoV();
  m_mainCamera.setWidth(m_width);
  m_mainCamera.setHeight(m_height);

  m_mainCamera.init();
}

void
AppTest::onUpdate(float deltaTime) {
  auto myGraphicsApi = g_graphicApi().instancePtr();

  ConstantBuffer1 meshData;
  meshData.mProjection = myGraphicsApi->matrixPolicy(m_mainCamera.getProjection());
  meshData.mView = myGraphicsApi->matrixPolicy(m_mainCamera.getView());

  ConstantBuffer2 cb;
  cb.mWorld = m_world;
  cb.vMeshColor = m_vMeshColor;

  try {
    //Clear the back buffer
    Vector4 rgba = { (87.0f / 255.0f), (35.0f / 255.0f), (100.0f / 255.0f), (255.0f) };
    myGraphicsApi->clearYourRenderTargetView(m_pRenderTargetView, rgba);

    // Clear the depth buffer to 1.0 (max depth)
    myGraphicsApi->clearYourDepthStencilView(m_pDepthStencil);

    myGraphicsApi->updateConstantBuffer(&meshData, m_pBufferCamera);
    myGraphicsApi->updateConstantBuffer(&cb, m_pBufferWorld);

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
    myGraphicsApi->setViewports(m_width, m_height);

    //Guardamos el input layout
    myGraphicsApi->setInputLayout(m_pVertexLayout);

    //Guardamos el vertex buffer
    myGraphicsApi->setVertexBuffer(m_mesh->m_pVertexBuffer);

    //Guardamos el index buffer
    myGraphicsApi->setIndexBuffer(m_mesh->m_pIndexBuffer);

    //Guardamos la topología
    myGraphicsApi->setPrimitiveTopology();

    myGraphicsApi->setShaders(m_pBothShaders);

    myGraphicsApi->setYourVSConstantBuffers(m_pBufferCamera, 0);
    myGraphicsApi->setYourVSConstantBuffers(m_pBufferWorld, 1);

    myGraphicsApi->setYourPSConstantBuffers(m_pBufferWorld, 1);

    myGraphicsApi->setYourVSConstantBuffers(m_tempBufferBones, 2);

    m_renderModel->drawModel(m_resourceManager, m_tempBufferBones);
  }
  catch (exception* e) {
    std::cout << "- - > " << e->what() << '\n';
    exit(1);
  }
  
  // Present our back buffer to our front buffer
  g_graphicApi().swapChainPresent();
}

void
AppTest::onCreate() {

  auto myGraphicsApi = g_graphicApi().instancePtr();

  onInitCamera();

  //Creamos el render target view
  m_pRenderTargetView.reset(myGraphicsApi->getDefaultBackBuffer());

  //Creamos el depth stencil view
  m_pDepthStencil.reset(myGraphicsApi->getDefaultDepthStencil());

  try {
    m_renderModel = new RenderModels();
    m_mesh.reset(new Mesh());
    m_resourceManager.reset(new ResourceManager());

    //Creamos el vertex shader y pixel shader.
    //DX_animation
    //DX_color
    m_pBothShaders.reset(myGraphicsApi->createShadersProgram(L"data/shaders/DX_animation.fx",
                                                             "VS",
                                                             L"data/shaders/DX_animation.fx",
                                                             "PS"));
    //Creamos el vertex shader y pixel shader.
    //m_pBothShaders = myGraphicsApi->createShadersProgram(L"data/shaders/OGL_VSAnim.fx",
    //                                                     "main",
    //                                                     L"data/shaders/OGL_PSAnim.fx",
    //                                                     "main");

    //Creamos el input layout 
    m_pVertexLayout.reset(myGraphicsApi->createInputLayout(m_pBothShaders));

    //Creamos el vertex buffer
    m_mesh->m_pVertexBuffer.reset(myGraphicsApi->createVertexBuffer(nullptr, sizeof(Matrices)));

    //Creamos el index buffer 
    m_mesh->m_pIndexBuffer.reset(myGraphicsApi->createIndexBuffer(nullptr, sizeof(ViewCB)));

    //Creamos los constant buffers para el shader
    m_pBufferCamera.reset(myGraphicsApi->createConstantBuffer(sizeof(ConstantBuffer1)));
    m_pBufferWorld.reset(myGraphicsApi->createConstantBuffer(sizeof(ConstantBuffer2)));
    m_pConstBufferBones = myGraphicsApi->createConstantBuffer(sizeof(ConstBuffBonesTransform));

    m_tempBufferBones.reset(m_pConstBufferBones);
  }
  catch (exception* e) {
    std::cout << "- - > " << e->what() << '\n';
    exit(1);
  }

  try {
    //m_resourceManager->initLoadModel("data/models/2B/2B.obj");
    //m_resourceManager->initLoadModel("data/models/pod/POD.obj");
    //m_resourceManager->initLoadModel("data/models/spartan/Spartan.fbx");
    m_resourceManager->initLoadModel("data/models/ugandan/Knuckles.fbx");
    //m_resourceManager->initLoadModel("data/models/grimoires/grimoires.fbx");

    //Esto solo es para guardar los huesos y las animaciones
    //En caso de que no, evitamos un error
    if (!m_resourceManager->getMeshes().empty()) {
      m_renderModel->setMeshBones(*m_resourceManager);

      if (!m_resourceManager->getModel()->m_vAnimationData.empty()) {
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
  //delete m_pBufferCamera;
  //delete m_pBufferWorld;
}

void
AppTest::onKeyboardDown(sf::Event param) {
  m_mainCamera.inputDetection(param);

  auto myGraphicsApi = g_graphicApi().instancePtr();

  ConstantBuffer1 cb;
  cb.mView = myGraphicsApi->matrixPolicy(m_mainCamera.getView());

  myGraphicsApi->updateConstantBuffer(&cb, m_pBufferCamera);
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
      m_mainCamera.getOriginalMousePos().x,
      m_mainCamera.getOriginalMousePos().y
    );

    m_mainCamera.mouseRotation();

    auto myGraphicsApi = g_graphicApi().instancePtr();

    ConstantBuffer1 cb;
    cb.mView = myGraphicsApi->matrixPolicy(m_mainCamera.getView());

    myGraphicsApi->updateConstantBuffer(&cb, m_pBufferCamera);
  }
}