#include <Windows.h>
#include <gaModels.h>
#include <gaSceneGraph.h>
#include <gaResourceManager.h>

#include "gaAppTest.h"

void
AppTest::onInitCamera() {
  //We initialize the identity matrix.
  m_world.identity();

  m_mainCamera.setLookAt(Vector3(0.0f, 1.0f, 0.0f));
  m_mainCamera.setEye(Vector3(0.0f, 0.0f, -700.0f));
  m_mainCamera.setUp(Vector3(0.0f, 1.0f, 0.0f));
  m_mainCamera.setFar();
  m_mainCamera.setNear();
  m_mainCamera.setFoV();
  m_mainCamera.setWidth(m_width);
  m_mainCamera.setHeight(m_height);

  m_mainCamera.startCamera();
}

void
AppTest::onUpdate(float deltaTime) {
  auto myGraphicsApi = g_graphicApi().instancePtr();
  auto mySceneGraph = SceneGraph::instancePtr();

  ConstantBuffer1 meshData;
  meshData.mProjection = myGraphicsApi->matrixPolicy(m_mainCamera.getProjection());
  meshData.mView = myGraphicsApi->matrixPolicy(m_mainCamera.getView());

  ConstantBuffer2 cb;
  cb.mWorld = m_world;
  cb.vMeshColor = m_vMeshColor;

  //Clear the back buffer.
  Vector4 rgba = { (87.0f / 255.0f), (35.0f / 255.0f), (100.0f / 255.0f), (255.0f) };
  myGraphicsApi->clearYourRenderTargetView(m_pRenderTargetView, rgba);

  // Clear the depth buffer to 1.0 (max depth).
  myGraphicsApi->clearYourDepthStencilView(m_pDepthStencil);

  //Update CB.
  myGraphicsApi->updateConstantBuffer(&meshData, m_pBufferCamera);
  myGraphicsApi->updateConstantBuffer(&cb, m_pBufferWorld);

  //Update the nodes info.
  mySceneGraph->update(deltaTime);
}

void
AppTest::onRender() {
  auto myGraphicsApi = g_graphicApi().instancePtr();
  auto mySceneGraph = SceneGraph::instancePtr();

  /***************************************************************************/
  /*
  * Sets.
  */
  /***************************************************************************/

  //We save the render targets.
  myGraphicsApi->setRenderTarget(m_pRenderTargetView, m_pDepthStencil);

  //We save a viewport.
  myGraphicsApi->setViewports(m_width, m_height);

  //We save the input layout.
  myGraphicsApi->setInputLayout(m_pVertexLayout);

  //We save the vertex buffer.
  myGraphicsApi->setVertexBuffer(m_mesh->m_pVertexBuffer);

  //We save the index buffer.
  myGraphicsApi->setIndexBuffer(m_mesh->m_pIndexBuffer);

  //We save the topology.
  myGraphicsApi->setPrimitiveTopology();

  myGraphicsApi->setShaders(m_pBothShaders);

  //VS CB
  myGraphicsApi->setYourVSConstantBuffers(m_pBufferCamera, 0);
  myGraphicsApi->setYourVSConstantBuffers(m_pBufferWorld, 1);

  //PS CB
  myGraphicsApi->setYourPSConstantBuffers(m_pBufferWorld, 1);

  //VS CB
  myGraphicsApi->setYourVSConstantBuffers(m_pCBufferBones, 2);

  //We keep the cb of the bones.
  myGraphicsApi->setConstBufferBones(m_pCBufferBones);

  //Render model
  mySceneGraph->render();
  
  // Present our back buffer to our front buffer
  myGraphicsApi->swapChainPresent();
}

void
AppTest::onCreate() {
  auto myGraphicsApi = g_graphicApi().instancePtr();

  m_mesh.reset(new Mesh());

  //We send the window to the API.
  myGraphicsApi->initDevice(m_sfmlWindow.getSystemHandle());

  onInitCamera();

  //We create the render target view.
  m_pRenderTargetView.reset(myGraphicsApi->getDefaultBackBuffer());

  //We create the depth stencil view.
  m_pDepthStencil.reset(myGraphicsApi->getDefaultDepthStencil());

  //We create the vertex shader and pixel shader.
  m_pBothShaders.reset(myGraphicsApi->createShadersProgram(L"data/shaders/DX_animation.fx",
                                                           "VS",
                                                           L"data/shaders/DX_animation.fx",
                                                           "PS"));
  //We create the vertex shader and pixel shader.
  //m_pBothShaders = myGraphicsApi->createShadersProgram(L"data/shaders/OGL_VSAnim.fx",
  //                                                     "main",
  //                                                     L"data/shaders/OGL_PSAnim.fx",
  //                                                     "main");

  //We create the input layout.
  m_pVertexLayout.reset(myGraphicsApi->createInputLayout(m_pBothShaders));

  //We create the vertex buffer.
  m_mesh->m_pVertexBuffer.reset(myGraphicsApi->createVertexBuffer(nullptr, sizeof(Matrices)));

  //We create the index buffer.
  m_mesh->m_pIndexBuffer.reset(myGraphicsApi->createIndexBuffer(nullptr, sizeof(ViewCB)));

  //We create the constant buffers for the shader.
  m_pBufferCamera.reset(myGraphicsApi->createConstantBuffer(sizeof(ConstantBuffer1)));
  m_pBufferWorld.reset(myGraphicsApi->createConstantBuffer(sizeof(ConstantBuffer2)));
  m_pCBufferBones.reset(myGraphicsApi->createConstantBuffer(sizeof(ConstBuffBonesTransform)));

  /***************************************************************************/
  /*
  * Loading models.
  * Set Node
  */
  /***************************************************************************/

  //createNodePod();
  //createNodeVela();
  //createNodeTwoB();
  //createNodeSpartan();
  //createNodeUgandan();
  createNodeGrimoires();
  createNodeRamlethalSwords();
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
AppTest::onKeyboardDown(sf::Event param, const float& deltaTime) {
  auto myGraphicsApi = g_graphicApi().instancePtr();

  m_mainCamera.inputDetection(param, deltaTime);

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

    m_mainCamera.setOriginalMousePos(m_mainCamera.getOriginalMousePos().x,
                                     m_mainCamera.getOriginalMousePos().y);

    m_mainCamera.mouseRotation();

    ConstantBuffer1 cb;
    cb.mView = myGraphicsApi->matrixPolicy(m_mainCamera.getView());

    myGraphicsApi->updateConstantBuffer(&cb, m_pBufferCamera);
  }
}

/*****************************************************************************/
/*
* Nodes.
*/
/*****************************************************************************/

void
AppTest::createNodePod() {
  auto mySceneGraph = SceneGraph::instancePtr();

  //Creating the component
  SPtr<Component> newComponent(new ResourceManager("data/models/pod/POD.obj"));

  //Creating actor
  SPtr<Actor> actor(new Actor("Pod"));
  actor->setIsSelected(true);
  actor->setComponent(newComponent);

  //Adding the actor to node root
  mySceneGraph->createNewActor(actor, SPtr<SceneNode>(nullptr));
}

void
AppTest::createNodeVela() {
  auto mySceneGraph = SceneGraph::instancePtr();

  //Creating the component
  SPtr<Component> newComponent(new ResourceManager("data/models/vela/Vela2.fbx"));

  //Creating actor
  SPtr<Actor> actor(new Actor("Vela"));
  actor->setIsSelected(true);
  actor->setComponent(newComponent);

  //Adding the actor to node root
  mySceneGraph->createNewActor(actor, SPtr<SceneNode>(nullptr));
}

void
AppTest::createNodeTwoB() {
  /*
  m_resourceInfo->loadTexture("data/textures/2B/2B_Diffuse.png");
  m_resourceInfo->loadTexture("data/textures/2B/2B_Alpha.png");
  m_resourceInfo->loadTexture("data/textures/2B/2B_HairAlpha.png");
  m_resourceInfo->loadTexture("data/textures/2B/2B_HairD.png");
  m_resourceInfo->loadTexture("data/textures/2B/2B_HairN.png");
  m_resourceInfo->loadTexture("data/textures/2B/2B_HairS.png");
  m_resourceInfo->loadTexture("data/textures/2B/2B_Normalfix2.png");
  m_resourceInfo->loadTexture("data/textures/2B/2B_Specular.png");
  */

  auto mySceneGraph = SceneGraph::instancePtr();

  //Creating the component
  SPtr<Component> newComponent(new ResourceManager("data/models/2B/2B.obj"));

  //Creating actor
  SPtr<Actor> actor(new Actor("2B"));
  actor->setIsSelected(true);
  actor->setComponent(newComponent);

  //Adding the actor to node root
  mySceneGraph->createNewActor(actor, SPtr<SceneNode>(nullptr));
}

void
AppTest::createNodeSpartan() {
  /*
  m_resourceInfo->loadTexture("data/textures/spartan/lambert1_Colour-Opacity.png");
  m_resourceInfo->loadTexture("data/textures/spartan/lambert1_Nrm.png");
  m_resourceInfo->loadTexture("data/textures/spartan/lambert1_Roughness.png");

  m_resourceInfo->loadTexture("data/textures/spartan/ODST_Shoulder_Mat_BaseColor.png");

  m_resourceInfo->loadTexture("data/textures/spartan/ODST_Shoulder_Mat_AO.png");
  m_resourceInfo->loadTexture("data/textures/spartan/ODST_Shoulder_Mat_Metallic.png");
  m_resourceInfo->loadTexture("data/textures/spartan/ODST_Shoulder_Mat_Normal.png");
  m_resourceInfo->loadTexture("data/textures/spartan/ODST_Shoulder_Mat_Roughness.png");
  m_resourceInfo->loadTexture("data/textures/spartan/ODST_Shoulder_Mat_Specular.png");

  m_resourceInfo->loadTexture("data/textures/spartan/Spartan_Arms_Mat_BaseColor.png");

  m_resourceInfo->loadTexture("data/textures/spartan/Spartan_Arms_Mat_AO.png");
  m_resourceInfo->loadTexture("data/textures/spartan/Spartan_Arms_Mat_Metallic.png");
  m_resourceInfo->loadTexture("data/textures/spartan/Spartan_Arms_Mat_Normal.png");
  m_resourceInfo->loadTexture("data/textures/spartan/Spartan_Arms_Mat_Roughness.png");
  m_resourceInfo->loadTexture("data/textures/spartan/Spartan_Arms_Mat_Specular.png");

  m_resourceInfo->loadTexture("data/textures/spartan/Spartan_Chest_Mat_BaseColor.png");

  m_resourceInfo->loadTexture("data/textures/spartan/Spartan_Chest_Mat_AO.png");
  m_resourceInfo->loadTexture("data/textures/spartan/Spartan_Chest_Mat_Metallic.png");
  m_resourceInfo->loadTexture("data/textures/spartan/Spartan_Chest_Mat_Normal.png");
  m_resourceInfo->loadTexture("data/textures/spartan/Spartan_Chest_Mat_Roughness.png");
  m_resourceInfo->loadTexture("data/textures/spartan/Spartan_Chest_Mat_Specular.png");

  m_resourceInfo->loadTexture("data/textures/spartan/Spartan_Ears_Mat_BaseColor.png");

  m_resourceInfo->loadTexture("data/textures/spartan/Spartan_Ears_Mat_AO.png");
  m_resourceInfo->loadTexture("data/textures/spartan/Spartan_Ears_Mat_Metallic.png");
  m_resourceInfo->loadTexture("data/textures/spartan/Spartan_Ears_Mat_Normal.png");
  m_resourceInfo->loadTexture("data/textures/spartan/Spartan_Ears_Mat_Roughness.png");
  m_resourceInfo->loadTexture("data/textures/spartan/Spartan_Ears_Mat_Specular.png");

  m_resourceInfo->loadTexture("data/textures/spartan/Spartan_Helmet_Mat_BaseColor.png");

  m_resourceInfo->loadTexture("data/textures/spartan/Spartan_Helmet_Mat_AO.png");
  m_resourceInfo->loadTexture("data/textures/spartan/Spartan_Helmet_Mat_Metallic.png");
  m_resourceInfo->loadTexture("data/textures/spartan/Spartan_Helmet_Mat_Normal.png");
  m_resourceInfo->loadTexture("data/textures/spartan/Spartan_Helmet_Mat_Roughness.png");
  m_resourceInfo->loadTexture("data/textures/spartan/Spartan_Helmet_Mat_Specular.png");

  m_resourceInfo->loadTexture("data/textures/spartan/Spartan_Legs_Mat_BaseColor.png");

  m_resourceInfo->loadTexture("data/textures/spartan/Spartan_Legs_Mat_AO.png");
  m_resourceInfo->loadTexture("data/textures/spartan/Spartan_Legs_Mat_Metallic.png");
  m_resourceInfo->loadTexture("data/textures/spartan/Spartan_Legs_Mat_Normal.png");
  m_resourceInfo->loadTexture("data/textures/spartan/Spartan_Legs_Mat_Roughness.png");
  m_resourceInfo->loadTexture("data/textures/spartan/Spartan_Legs_Mat_Specular.png");

  m_resourceInfo->loadTexture("data/textures/spartan/Spartan_Undersuit_Mat_BaseColor.png");

  m_resourceInfo->loadTexture("data/textures/spartan/Spartan_Undersuit_Mat_AO.png");
  m_resourceInfo->loadTexture("data/textures/spartan/Spartan_Undersuit_Mat_Metallic.png");
  m_resourceInfo->loadTexture("data/textures/spartan/Spartan_Undersuit_Mat_Normal.png");
  m_resourceInfo->loadTexture("data/textures/spartan/Spartan_Undersuit_Mat_Roughness.png");
  m_resourceInfo->loadTexture("data/textures/spartan/Spartan_Undersuit_Mat_Specular.png");
  */
  auto mySceneGraph = SceneGraph::instancePtr();

  //Creating the component
  SPtr<Component> newComponent(new ResourceManager("data/models/spartan/Spartan.fbx"));

  //Creating actor
  SPtr<Actor> actor(new Actor("Spartan"));
  actor->setIsSelected(true);
  actor->setComponent(newComponent);

  //Adding the actor to node root
  mySceneGraph->createNewActor(actor, SPtr<SceneNode>(nullptr));
}

void
AppTest::createNodeUgandan() {
  auto mySceneGraph = SceneGraph::instancePtr();

  //Creating the component
  SPtr<Component> newComponent(new ResourceManager("data/models/ugandan/Knuckles.fbx"));

  //Creating actor
  SPtr<Actor> actor(new Actor("Ugandan"));
  actor->setIsSelected(true);
  actor->setComponent(newComponent);

  //Adding the actor to node root
  mySceneGraph->createNewActor(actor, SPtr<SceneNode>(nullptr));
}

void 
AppTest::createNodeGrimoires() {
  auto mySceneGraph = SceneGraph::instancePtr();

  //Creating the component
  SPtr<Component> newComponent(new ResourceManager("data/models/grimoires/grimoires.fbx"));

  //Creating actor
  SPtr<Actor> actor(new Actor("Grimoires"));
  actor->setIsSelected(true);
  actor->setComponent(newComponent);

  //Adding the actor to node root
  mySceneGraph->createNewActor(actor, SPtr<SceneNode>(nullptr));
}

void 
AppTest::createNodeRamlethalSwords() {
  auto mySceneGraph = SceneGraph::instancePtr();

  //Creating the component
  SPtr<Component> newComponent(new ResourceManager("data/models/ramlethal/Ramlethal Sword.fbx"));

  //Creating actor
  SPtr<Actor> actor(new Actor("Sword"));
  actor->setIsSelected(true);
  actor->setComponent(newComponent);

  //Adding the actor to node root
  mySceneGraph->createNewActor(actor, SPtr<SceneNode>(nullptr));
}