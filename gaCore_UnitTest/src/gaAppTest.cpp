#include <Windows.h>
#include <iostream>
#include <gaModels.h>
#include <gaResourceManager.h>
#include <gaRenderModels.h>
#include <gaSceneGraph.h>

#include "gaAppTest.h"

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

  //Clear the back buffer
  Vector4 rgba = { (87.0f / 255.0f), (35.0f / 255.0f), (100.0f / 255.0f), (255.0f) };
  myGraphicsApi->clearYourRenderTargetView(m_pRenderTargetView, rgba);

  // Clear the depth buffer to 1.0 (max depth)
  myGraphicsApi->clearYourDepthStencilView(m_pDepthStencil);

  //Update CB
  myGraphicsApi->updateConstantBuffer(&meshData, m_pBufferCamera);
  myGraphicsApi->updateConstantBuffer(&cb, m_pBufferWorld);

  m_renderModel->update(*m_resourceInfo, deltaTime);
}

void
AppTest::onRender() {
  auto myGraphicsApi = g_graphicApi().instancePtr();

  /***************************************************************************/
  /*
  * Sets.
  */
  /***************************************************************************/

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

  //VS CB
  myGraphicsApi->setYourVSConstantBuffers(m_pBufferCamera, 0);
  myGraphicsApi->setYourVSConstantBuffers(m_pBufferWorld, 1);

  //PS CB
  myGraphicsApi->setYourPSConstantBuffers(m_pBufferWorld, 1);

  //VS CB
  myGraphicsApi->setYourVSConstantBuffers(m_tempBufferBones, 2);

  //Guardamos el cb de los huesos
  myGraphicsApi->setConstBufferBones(m_tempBufferBones);

  //Render model
  m_renderModel->drawModel();
  
  // Present our back buffer to our front buffer
  g_graphicApi().swapChainPresent();
}

void
AppTest::onCreate() {
  auto myGraphicsApi = g_graphicApi().instancePtr();

  m_renderModel = new RenderModels();
  m_mesh.reset(new Mesh());
  m_resourceInfo.reset(new ResourceManager());

  //Mandamos la ventana a la API
  myGraphicsApi->initDevice(m_sfmlWindow.getSystemHandle());

  onInitCamera();

  //Creamos el render target view
  m_pRenderTargetView.reset(myGraphicsApi->getDefaultBackBuffer());

  //Creamos el depth stencil view
  m_pDepthStencil.reset(myGraphicsApi->getDefaultDepthStencil());

  //Creamos el vertex shader y pixel shader.
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

  /***************************************************************************/
  /*
  * Loading models.
  */
  /***************************************************************************/

  //m_resourceInfo->initLoadModel("data/models/2B/2B.obj");
  //m_resourceInfo->initLoadModel("data/models/pod/POD.obj");
  //m_resourceInfo->initLoadModel("data/models/vela/Vela2.fbx");
  //m_resourceInfo->initLoadModel("data/models/spartan/Spartan.fbx");
  //m_resourceInfo->initLoadModel("data/models/ugandan/Knuckles.fbx");
  //m_resourceInfo->initLoadModel("data/models/grimoires/grimoires.fbx");

  //createActor();

  /***************************************************************************/
  /*
  * Check animations.
  */
  /***************************************************************************/

  //Esto solo es para guardar los huesos y las animaciones
  //En caso de que no, evitamos un error
  m_renderModel->m_meshBones.resize(m_resourceInfo->getMeshes().size());

  if (!m_resourceInfo->getModel()->getAnimData().empty()) {
    m_renderModel->m_currentAnimation = m_resourceInfo->getModel()->getAnimData()[0];
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
  auto myGraphicsApi = g_graphicApi().instancePtr();

  m_mainCamera.inputDetection(param);

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
    auto myGraphicsApi = g_graphicApi().instancePtr();

    m_mainCamera.setOriginalMousePos
    (
      m_mainCamera.getOriginalMousePos().x,
      m_mainCamera.getOriginalMousePos().y
    );

    m_mainCamera.mouseRotation();

    ConstantBuffer1 cb;
    cb.mView = myGraphicsApi->matrixPolicy(m_mainCamera.getView());

    myGraphicsApi->updateConstantBuffer(&cb, m_pBufferCamera);
  }
}

void 
AppTest::createActor() {
  /*SPtr<Actor> actor(new Actor("Actor##"));

  actor->setComponent(TYPE_COMPONENTS::kRenderModel);
  SPtr<SceneNode> node = SceneGraph::instance().createNewActor
                                                (actor, SPtr<SceneNode>(nullptr));

  auto rModel = reinterpret_cast<RenderModels*>
    (tempActor->getComponent(TYPE_COMPONENTS::kRenderModel).get());

  rModel->setModel(g_resourceManager().getModel());*/
}
