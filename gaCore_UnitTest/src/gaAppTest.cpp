#include <Windows.h>
#include <gaModels.h>
#include <gaSceneGraph.h>
#include <gaStaticMesh.h>
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

  // Clear the depth buffer to 1.0 (max depth).
  myGraphicsApi->clearYourDepthStencilView(m_pDepthStencil);
  
  //Update the nodes info.
  mySceneGraph->update(deltaTime);
}

void
AppTest::onRender() {
  auto myGraphicsApi = g_graphicApi().instancePtr();

  /***************************************************************************/
  /*
  * Sets.
  */
  /***************************************************************************/

  //We save the render targets.
  setRTGbufferPass();
  setRT_SSAO_Pass();
  setBlurH_Pass(m_pSSAO_RT->getRenderTexture(0));
  setBlurV_Pass(m_pSSAO_RT->getRenderTexture(0));
  setAdditionPass(m_pBlurH_RT->getRenderTexture(0), m_pBlurV_RT->getRenderTexture(0));

  for (uint32 i = 0; i < 2; ++i) {
    setBlurH_Pass(m_pAdditionRT->getRenderTexture(0));
    setBlurV_Pass(m_pAdditionRT->getRenderTexture(0));
    setAdditionPass(m_pBlurH_RT->getRenderTexture(0), m_pBlurV_RT->getRenderTexture(0));
  }

  setLightningPass();

  //We save a viewport.
  myGraphicsApi->setViewports(m_width, m_height);
  
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
  /*m_pGBufferShader.reset(myGraphicsApi->createShadersProgram(L"data/shaders/DX_animation.fx",
                                                            "VS",
                                                           L"data/shaders/DX_animation.fx",
                                                            "PS"));*/
  m_pGBufferShader.reset(myGraphicsApi->createShadersProgram(L"data/shaders/DX_gBuffer.fx",
                                                             "vs_gBuffer",
                                                             L"data/shaders/DX_gBuffer.fx",
                                                             "ps_gBuffer"));

  m_pSSAO_Shader.reset(myGraphicsApi->createShadersProgram(L"data/shaders/DX_screenAlignedQuad.fx",
                                                           "vs_ssAligned",
                                                           L"data/shaders/DX_SSAO.fx",
                                                           "ps_ssao"));

  m_pBlurH_Shader.reset(myGraphicsApi->createShadersProgram(L"data/shaders/DX_screenAlignedQuad.fx",
                                                            "vs_ssAligned",
                                                            L"data/shaders/DX_gaussyan_blur.fx",
                                                            "ps_gaussian_blurH"));

  m_pBlurV_Shader.reset(myGraphicsApi->createShadersProgram(L"data/shaders/DX_screenAlignedQuad.fx",
                                                            "vs_ssAligned",
                                                            L"data/shaders/DX_gaussyan_blur.fx",
                                                            "ps_gaussian_blurV"));

  m_pLightningShader.reset(myGraphicsApi->createShadersProgram(L"data/shaders/DX_screenAlignedQuad.fx",
                                                               "vs_ssAligned",
                                                               L"data/shaders/DX_lightningPS.fx",
                                                               "ps_main"));

  m_pAdditionShader.reset(myGraphicsApi->createShadersProgram(L"data/shaders/DX_screenAlignedQuad.fx",
                                                              "vs_ssAligned",
                                                              L"data/shaders/DX_AdditionPS.fx",
                                                              "Add"));

  //We create the input layout.
  m_pVertexLayout.reset(myGraphicsApi->createInputLayout(m_pGBufferShader));

  //We create the vertex buffer.
  m_mesh->m_pVertexBuffer.reset(myGraphicsApi->createVertexBuffer(nullptr, sizeof(Matrices)));

  //We create the index buffer.
  m_mesh->m_pIndexBuffer.reset(myGraphicsApi->createIndexBuffer(nullptr, sizeof(ViewCB)));

  /*
  * C R E A T E
  * B U F F E R S
  * Z O N E
  */
  m_pBufferCamera.reset(myGraphicsApi->createConstantBuffer(sizeof(ConstantBuffer1)));
  m_pBufferWorld.reset(myGraphicsApi->createConstantBuffer(sizeof(ConstantBuffer2)));
  m_pCBufferBones.reset(myGraphicsApi->createConstantBuffer(sizeof(ConstBuffBonesTransform)));

  m_pCB_SSAO.reset(myGraphicsApi->createConstantBuffer(sizeof(cbSSAO)));
  m_pCB_ViewPortDimension.reset(myGraphicsApi->createConstantBuffer(sizeof(cbViewportDimension)));
  m_pCB_Lightning.reset(myGraphicsApi->createConstantBuffer(sizeof(cbLightning)));
  m_pCB_MipLevels.reset(myGraphicsApi->createConstantBuffer(sizeof(cbMipLevels)));

  /*
  * C R E A T E
  * R E N D E R 
  * T A R G E TS
  * Z O N E
  */

  m_pGbufferRT = make_shared<RenderTarget>();
  m_pSSAO_RT = make_shared<RenderTarget>();
  m_pBlurH_RT = make_shared<RenderTarget>();
  m_pBlurV_RT = make_shared<RenderTarget>();
  m_pAdditionRT = make_shared<RenderTarget>();
  m_pLightningRT = make_shared<RenderTarget>();

  m_pGbufferRT = myGraphicsApi->createRenderTarget(m_width, m_height, 1, 4);
  m_pSSAO_RT = myGraphicsApi->createRenderTarget(m_width, m_height);
  m_pBlurH_RT = myGraphicsApi->createRenderTarget(m_width, m_height);
  m_pBlurV_RT = myGraphicsApi->createRenderTarget(m_width, m_height);
  m_pAdditionRT = myGraphicsApi->createRenderTarget(m_width, m_height);
  m_pLightningRT = myGraphicsApi->createRenderTarget(m_width, m_height);

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
  //createNodeRamlethalSwords();

  /*
  * C R E A T E
  * S A M P L E R
  * Z O N E
  */
  m_pSampler = myGraphicsApi->createSamplerState();
  myGraphicsApi->setSamplerState(m_pSampler, 0);

  //Create the one face of cube.
  createSAQ();
}

void
AppTest::onDestroySystem() {
  //delete m_pRenderTargetView;
  //delete m_pDepthStencil;
  //delete m_pVertexLayout;
  //delete m_pGBufferShader;
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
  auto myRSRCMG = ResourceManager::instancePtr();

  SPtr<Models> myModel = myRSRCMG->load<Models>("data/models/pod/POD.obj");

  SPtr<StaticMesh> myStaticMesh = make_shared<StaticMesh>();
  myStaticMesh->m_pModel = myModel;

  m_vStaticMeshes.push_back(myStaticMesh);

  //Creating the component
  SPtr<Component> newComponent(myStaticMesh);

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
  auto myRSRCMG = ResourceManager::instancePtr();

  SPtr<Models> myModel = myRSRCMG->load<Models>("data/models/vela/Vela2.fbx");

  SPtr<StaticMesh> myStaticMesh = make_shared<StaticMesh>();
  myStaticMesh->m_pModel = myModel;

  m_vStaticMeshes.push_back(myStaticMesh);

  //Creating the component
  SPtr<Component> newComponent(myStaticMesh);

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

  SPtr<Models> myModel = make_shared<Models>();
  myModel->loadFromFile("data/models/2B/2B.obj");

  SPtr<StaticMesh> myStaticMesh = make_shared<StaticMesh>();
  myStaticMesh->m_pModel = myModel;

  m_vStaticMeshes.push_back(myStaticMesh);

  //Creating the component
  SPtr<Component> newComponent(myStaticMesh);

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
  auto myRSRCMG = ResourceManager::instancePtr();

  SPtr<Models> myModel = myRSRCMG->load<Models>("data/models/spartan/Spartan.fbx");

  SPtr<StaticMesh> myStaticMesh = make_shared<StaticMesh>();
  myStaticMesh->m_pModel = myModel;

  m_vStaticMeshes.push_back(myStaticMesh);

  //Creating the component
  SPtr<Component> newComponent(myStaticMesh);

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
  auto myRSRCMG = ResourceManager::instancePtr();

  SPtr<Models> myModel = myRSRCMG->load<Models>("data/models/ugandan/Knuckles.fbx");

  SPtr<StaticMesh> myStaticMesh = make_shared<StaticMesh>();
  myStaticMesh->m_pModel = myModel;

  m_vStaticMeshes.push_back(myStaticMesh);

  //Creating the component
  SPtr<Component> newComponent(myStaticMesh);

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
  auto myRSRCMG = ResourceManager::instancePtr();

  SPtr<Models> myModel = myRSRCMG->load<Models>("data/models/grimoires/grimoires.fbx");

  SPtr<StaticMesh> myStaticMesh = make_shared<StaticMesh>();
  myStaticMesh->m_pModel = myModel;

  m_vStaticMeshes.push_back(myStaticMesh);

  //Creating the component
  SPtr<Component> newComponent(myStaticMesh);

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
  auto myRSRCMG = ResourceManager::instancePtr();

  SPtr<Models> myModel = myRSRCMG->load<Models>("data/models/ramlethal/Ramlethal Sword.fbx");

  SPtr<StaticMesh> myStaticMesh = make_shared<StaticMesh>();
  myStaticMesh->m_pModel = myModel;

  m_vStaticMeshes.push_back(myStaticMesh);

  //Creating the component
  SPtr<Component> newComponent(myStaticMesh);

  //Creating actor
  SPtr<Actor> actor(new Actor("Sword"));
  actor->setIsSelected(true);
  actor->setComponent(newComponent);

  //Adding the actor to node root
  mySceneGraph->createNewActor(actor, SPtr<SceneNode>(nullptr));
}

void
AppTest::createSAQ() {
  auto myGraphicsApi = g_graphicApi().instancePtr();
  Models* newModel = new Models();
  Mesh* newMesh = new Mesh();

  Vertex* meshVertex = new Vertex[4];
  meshVertex[0].position = { -1.0f, -1.0f, 0.0f, 1.0f };
  meshVertex[1].position = { -1.0f,  1.0f, 0.0f, 1.0f };
  meshVertex[2].position = { 1.0f, -1.0f, 0.0f, 1.0f };
  meshVertex[3].position = { 1.0f,  1.0f, 0.0f, 1.0f };

  meshVertex[0].normal = { 0.0f, 0.0f, -1.0f };
  meshVertex[1].normal = { 0.0f, 0.0f, -1.0f };
  meshVertex[2].normal = { 0.0f, 0.0f, -1.0f };
  meshVertex[3].normal = { 0.0f, 0.0f, -1.0f };

  meshVertex[0].texCoords = { 0.0f, 1.0f };
  meshVertex[1].texCoords = { 0.0f, 0.0f };
  meshVertex[2].texCoords = { 1.0f, 1.0f };
  meshVertex[3].texCoords = { 1.0f, 0.0f };

  uint32* meshIndex = new uint32[2 * 3];
  meshIndex[0] = 0;
  meshIndex[1] = 1;
  meshIndex[2] = 2;
  meshIndex[3] = 2;
  meshIndex[4] = 1;
  meshIndex[5] = 3;

  SPtr<Vertex> spVetexData(meshVertex);
  newMesh->setVertexData(spVetexData);

  newMesh->m_pVertexBuffer.reset(myGraphicsApi->createVertexBuffer(meshVertex, 
                                                                   sizeof(Vertex) * 4));

  SPtr<uint32> spIndex(meshIndex);
  newMesh->setIndex(spIndex);
  newMesh->setIndexNum(6);

  SPtr<IndexBuffer> tempIB(myGraphicsApi->createIndexBuffer(meshIndex, sizeof(uint32) * 6));
  newMesh->setIndexBuffer(tempIB);

  newModel->addNewMesh(*newMesh);
  m_SAQ.reset(newModel);
}

/*****************************************************************************/
/*
* Set.
*/
/*****************************************************************************/

void 
AppTest::setSAQ() {
  auto myGraphicsApi = g_graphicApi().instancePtr();
  auto mesh = m_SAQ->getMesh(0);

  myGraphicsApi->setVertexBuffer(mesh.getVertexBuffer());
  myGraphicsApi->setIndexBuffer(mesh.getIndexBuffer());
  myGraphicsApi->drawIndex(6);
}

void
AppTest::setRTGbufferPass() {
  auto myGraphicsApi = g_graphicApi().instancePtr();
  auto mySceneGraph = SceneGraph::instancePtr();

  //We save the input layout.
  myGraphicsApi->setInputLayout(m_pVertexLayout);

  //We save the vertex buffer.
  myGraphicsApi->setVertexBuffer(m_mesh->m_pVertexBuffer);

  //We save the index buffer.
  myGraphicsApi->setIndexBuffer(m_mesh->m_pIndexBuffer);

  //We save the topology.
  myGraphicsApi->setPrimitiveTopology();

  //We keep the cb of the bones.
  myGraphicsApi->setConstBufferBones(m_pCBufferBones);

  //Here starts the TOTSUGEKI
  myGraphicsApi->setRenderTarget(m_pGbufferRT, m_pDepthStencil);
  myGraphicsApi->setShaders(m_pGBufferShader);

  //VS CB
  myGraphicsApi->setYourVSConstantBuffers(m_pBufferCamera, 0);
  myGraphicsApi->setYourVSConstantBuffers(m_pBufferWorld, 1);

  //Animation
  //myGraphicsApi->setYourVSConstantBuffers(m_pCBufferBones, 2);
  //PS CB
  //myGraphicsApi->setYourPSConstantBuffers(m_pBufferWorld, 1);

  myGraphicsApi->clearYourRenderTarget(m_pGbufferRT, m_rgba);

  ConstantBuffer1 meshData;
  meshData.mProjection = myGraphicsApi->matrixPolicy(m_mainCamera.getProjection());
  meshData.mView = myGraphicsApi->matrixPolicy(m_mainCamera.getView());

  ConstantBuffer2 cb;
  cb.mWorld = m_world;
  cb.objectPosition = { 0.0f, 0.0f, 0.0f };

  //Update CB.
  myGraphicsApi->updateConstantBuffer(&meshData, m_pBufferCamera);
  myGraphicsApi->updateConstantBuffer(&cb, m_pBufferWorld);

  //Render model
  mySceneGraph->render();
}

void
AppTest::setRT_SSAO_Pass() {
  auto myGraphicsApi = g_graphicApi().instancePtr();

  myGraphicsApi->setRenderTarget(m_pSSAO_RT);
  myGraphicsApi->setShaders(m_pSSAO_Shader);

  //VS CB
  myGraphicsApi->setYourVSConstantBuffers(m_pBufferCamera, 0);
  myGraphicsApi->setYourVSConstantBuffers(m_pBufferWorld, 1);

  //PS CB
  myGraphicsApi->setYourPSConstantBuffers(m_pCB_SSAO, 2);

  //Clear
  myGraphicsApi->clearYourRenderTarget(m_pSSAO_RT, m_rgba);

  //Update CB
  cbSSAO ssaoData;
  ssaoData.mBias = 0.08000f;
  ssaoData.mIntensity = 2.0f;
  ssaoData.mNothing = { 0.0f, 0.0f };
  ssaoData.mSample_radius = 10.0f;
  ssaoData.mScale = 1.0f;

  ssaoData.mViewportDimensions = { (float)m_width, (float)m_height };

  myGraphicsApi->updateConstantBuffer(&ssaoData, m_pCB_SSAO);

  //Set textures
  myGraphicsApi->setShaderResourceView(m_pGbufferRT->getRenderTexture(2), 0);
  myGraphicsApi->setShaderResourceView(m_pGbufferRT->getRenderTexture(1), 1);

  setSAQ();
}

void
AppTest::setBlurH_Pass(void* texture) {
  auto myGraphicsApi = g_graphicApi().instancePtr();

  myGraphicsApi->setRenderTarget(m_pBlurH_RT);
  myGraphicsApi->setShaders(m_pBlurH_Shader);

  //VS CB
  myGraphicsApi->setYourVSConstantBuffers(m_pBufferCamera, 0);
  myGraphicsApi->setYourVSConstantBuffers(m_pBufferWorld, 1);

  //PS CB
  myGraphicsApi->setYourPSConstantBuffers(m_pCB_ViewPortDimension, 2);

  //Clear RT
  myGraphicsApi->clearYourRenderTarget(m_pBlurH_RT, m_rgba);

  //Update CB
  cbViewportDimension viewportDimenData;
  viewportDimenData.viewportDimensions = { (float)m_width, (float)m_height };
  viewportDimenData.mNothing = { 0.0f, 0.0f };

  myGraphicsApi->updateConstantBuffer(&viewportDimenData, m_pCB_ViewPortDimension);

  //Set texture
  myGraphicsApi->setShaderResourceView(texture, 0);

  setSAQ();
}

void
AppTest::setBlurV_Pass(void* texture) {
  auto myGraphicsApi = g_graphicApi().instancePtr();

  myGraphicsApi->setRenderTarget(m_pBlurV_RT);
  myGraphicsApi->setShaders(m_pBlurV_Shader);

  //VS CB
  myGraphicsApi->setYourVSConstantBuffers(m_pBufferCamera, 0);
  myGraphicsApi->setYourVSConstantBuffers(m_pBufferWorld, 1);

  //PS CB
  myGraphicsApi->setYourPSConstantBuffers(m_pCB_ViewPortDimension, 2);

  //Clear RT
  myGraphicsApi->clearYourRenderTarget(m_pBlurV_RT, m_rgba);

  //Update CB
  cbViewportDimension viewportDimenData;
  viewportDimenData.viewportDimensions = { (float)m_width, (float)m_height };
  viewportDimenData.mNothing = { 0.0f, 0.0f };

  myGraphicsApi->updateConstantBuffer(&viewportDimenData, m_pCB_ViewPortDimension);

  //Set textures
  myGraphicsApi->setShaderResourceView(texture, 0);

  setSAQ();
}

void 
AppTest::setAdditionPass(void* texture1, void* texture2) {
  auto myGraphicsApi = g_graphicApi().instancePtr();

  myGraphicsApi->setRenderTarget(m_pAdditionRT);
  myGraphicsApi->setShaders(m_pAdditionShader);

  //VS CB
  myGraphicsApi->setYourVSConstantBuffers(m_pBufferCamera, 0);
  myGraphicsApi->setYourVSConstantBuffers(m_pBufferWorld, 1);

  //PS CB
  myGraphicsApi->setYourPSConstantBuffers(m_pCB_MipLevels, 2);

  //Clean RT
  myGraphicsApi->clearYourRenderTarget(m_pAdditionRT, m_rgba);

  //Update CB
  cbMipLevels mipLevelsData;
  mipLevelsData.mipLevel0 = 0;
  mipLevelsData.mipLevel1 = 0;
  mipLevelsData.mipLevel2 = 0;
  mipLevelsData.mipLevel3 = 0;

  myGraphicsApi->updateConstantBuffer(&mipLevelsData, m_pCB_MipLevels);

  //Set textures
  myGraphicsApi->setShaderResourceView(texture1, 0);
  myGraphicsApi->setShaderResourceView(texture2, 1);

  setSAQ();
}

void 
AppTest::setLightningPass() {
  auto myGraphicsApi = g_graphicApi().instancePtr();

  myGraphicsApi->setRenderTarget(m_pRenderTargetView);
  myGraphicsApi->setShaders(m_pLightningShader);

  //VS CB
  myGraphicsApi->setYourVSConstantBuffers(m_pBufferCamera, 0);
  myGraphicsApi->setYourVSConstantBuffers(m_pBufferWorld, 1);

  //PS CB
  myGraphicsApi->setYourPSConstantBuffers(m_pBufferCamera, 0);
  myGraphicsApi->setYourPSConstantBuffers(m_pBufferWorld, 1);
  myGraphicsApi->setYourPSConstantBuffers(m_pCB_Lightning, 2);

  //Clean RT
  myGraphicsApi->clearYourRenderTargetView(m_pRenderTargetView, m_rgba);

  //Update CB
  cbLightning lightningData;
  lightningData.emissiveIntensity = 1.0f;
  lightningData.lightIntensity0 = 2.0f;
  lightningData.lightPos0 = {0.0f, 0.0f, 0.0f};
  lightningData.vViewPosition = m_mainCamera.getCamEye();

  myGraphicsApi->updateConstantBuffer(&lightningData, m_pCB_Lightning);

  //Set textures
  myGraphicsApi->setShaderResourceView(m_pGbufferRT->getRenderTexture(0), 0);
  myGraphicsApi->setShaderResourceView(m_pGbufferRT->getRenderTexture(1), 1);
  myGraphicsApi->setShaderResourceView(m_pGbufferRT->getRenderTexture(3), 2);
  myGraphicsApi->setShaderResourceView(m_pAdditionRT->getRenderTexture(0), 3);
  myGraphicsApi->setShaderResourceView(m_pGbufferRT->getRenderTexture(2), 4);

  setSAQ();
}