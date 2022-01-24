#include <gaGraphicsApi.h>
#include <gaPlane.h>
#include <gaRenderTarget.h>
#include <gaBaseInterface.h>
#include <gaStaticMesh.h>

#include "gaDeferredRendering.h"

namespace gaEngineSDK {
  void 
  DeferredRendering::init(uint32 width, uint32 height) {
    auto myGraphicsApi = g_graphicApi().instancePtr();

    m_mesh.reset(new Mesh());
    
    m_width = width;
    m_height = height;

    defaultCamera();

    //We create the render target view.
    m_pRenderTargetView.reset(myGraphicsApi->getDefaultBackBuffer());

    //We create the depth stencil view.
    m_pDepthStencil.reset(myGraphicsApi->getDefaultDepthStencil());

    /*
    * L O A D
    * S H A D E R S
    * Z O N E
    */
    loadShadersFiles();

    /*
    * C R E A T E
    * I N P U T
    * L A Y O U T
    * Z O N E
    */
    m_pVertexLayout.reset(myGraphicsApi->createInputLayout(m_pGBuffer_Shader));

    m_pDepthLayout.reset(myGraphicsApi->createInputLayout(m_pDepth_Shader));


    /*
    * ? ? ?
    */
    //We create the vertex buffer.
    m_mesh->m_pVertexBuffer.reset(myGraphicsApi->createVertexBuffer(nullptr, sizeof(Matrices)));

    //We create the index buffer.
    m_mesh->m_pIndexBuffer.reset(myGraphicsApi->createIndexBuffer(nullptr, sizeof(ViewCB)));
    /*
    * ? ? ?
    */


    /*
    * C R E A T E
    * B U F F E R S
    * Z O N E
    */
    createBuffers();

    /*
    * C R E A T E
    * R E N D E R
    * T A R G E TS
    * Z O N E
    */
    createRenderTargets();

    /*
    * C R E A T E
    * S A M P L E R
    * Z O N E
    */
    createSamplers();

    /*
    * S E T
    * P A S S
    * I N F O
    */
    setGBuffer();

    /*
    * C R E A T E
    * L I G H T
    */
    defaultLight();

    //Create the one face of cube.
    m_mySAQ.reset(new Plane());
    m_mySAQ->createSAQ();
  }

  void
  DeferredRendering::update(const float& deltaTime) {
    auto myGraphicsApi = g_graphicApi().instancePtr();

    /*
    * I N P U T S
    */
    keyboardButtons(deltaTime);
    mouseRotation(deltaTime);

    // Clear the depth buffer to 1.0 (max depth).
    myGraphicsApi->clearYourDepthStencilView(m_pDepthStencil);
  }

  void
  DeferredRendering::render() {
    auto myGraphicsApi = g_graphicApi().instancePtr();

    /*
    * P A S S E S
    * Z O N E
    */
    
    //Shadow
    depthPass();

    //Geometry
    gbufferPass();

    //Ambient occlusion and blur
    CS_SSAO();
    csBlurHPass(m_pCompSSAO_RT->getRenderTexture(0));
    csBlurVPass(m_pCompBlurH_RT->getRenderTexture(0));

    //Light
    lightningPass();

    //We save a viewport.
    myGraphicsApi->setViewports(m_width, m_height);
  }

  void 
  DeferredRendering::resize() { }

  /***************************************************************************/
  /**
  * Inputs.
  */
  /***************************************************************************/
  void 
  DeferredRendering::keyboardButtons(const float& deltaTime) {
    auto myInputs = g_baseInputs().instancePtr();
    auto myGraphicsApi = g_graphicApi().instancePtr();
    auto myInterface = g_baseInterface().instancePtr();

    if (!(myInterface->m_touchingImGui)) {
      //Normal movement
      if (myInputs->getKey(KEYBOARD::kW)) {
        m_mainCamera.move(KEYBOARD::kW, deltaTime);
      }
      else if (myInputs->getKey(KEYBOARD::kS)) {
        m_mainCamera.move(KEYBOARD::kS, deltaTime);
      }
      else if (myInputs->getKey(KEYBOARD::kA)) {
        m_mainCamera.move(KEYBOARD::kA, deltaTime);
      }
      else if (myInputs->getKey(KEYBOARD::kD)) {
        m_mainCamera.move(KEYBOARD::kD, deltaTime);
      }
      else if (myInputs->getKey(KEYBOARD::kQ)) {
        m_mainCamera.move(KEYBOARD::kQ, deltaTime);
      }
      else if (myInputs->getKey(KEYBOARD::kE)) {
        m_mainCamera.move(KEYBOARD::kE, deltaTime);
      }
      //Roll, Yaw, Pitch
      else if (myInputs->getKey(KEYBOARD::kUP)) {
        m_mainCamera.inputDetection(KEYBOARD::kUP, deltaTime);
      }
      else if (myInputs->getKey(KEYBOARD::kDOWN)) {
        m_mainCamera.inputDetection(KEYBOARD::kDOWN, deltaTime);
      }
      else if (myInputs->getKey(KEYBOARD::kLEFT)) {
        m_mainCamera.inputDetection(KEYBOARD::kLEFT, deltaTime);
      }
      else if (myInputs->getKey(KEYBOARD::kRIGHT)) {
        m_mainCamera.inputDetection(KEYBOARD::kRIGHT, deltaTime);
      }
      else if (myInputs->getKey(KEYBOARD::kZ)) {
        m_mainCamera.inputDetection(KEYBOARD::kZ, deltaTime);
      }
      else if (myInputs->getKey(KEYBOARD::kC)) {
        m_mainCamera.inputDetection(KEYBOARD::kC, deltaTime);
      }
    }

    cbCamera cameraInfo;
    cameraInfo.mView = myGraphicsApi->matrixPolicy(m_mainCamera.getView());

    myGraphicsApi->updateConstantBuffer(&cameraInfo, m_pCB_BufferCamera);
  }

  void
  DeferredRendering::mouseRotation(const float& deltaTime) {
    auto myGraphicsApi = g_graphicApi().instancePtr();
    auto myInputs = g_baseInputs().instancePtr();
    auto myInterface = g_baseInterface().instancePtr();

    if ((myInputs->getMouseBtn(MOUSE_BUTTON::kLeft)) && (!myInterface->m_touchingImGui)) {
      m_mainCamera.mouseRotation(deltaTime);
    }

    cbCamera cameraInfo;
    cameraInfo.mView = myGraphicsApi->matrixPolicy(m_mainCamera.getView());
    myGraphicsApi->updateConstantBuffer(&cameraInfo, m_pCB_BufferCamera);
  }

  void
  DeferredRendering::onLeftMouseBtnDown() {
    Vector2i position = Mouse::getPosition();

    m_mainCamera.setOriginalMousePos(position.x, position.y);
  }

  /***************************************************************************/
  /**
  * Cameras.
  */
  /***************************************************************************/

  void
  DeferredRendering::defaultCamera() {
    /*
    * M A I N
    * C A M E R A
    * I N F O
    */
    m_mainCamera.setLookAt(Vector3(0.0f, 0.0f, 0.0f));
    m_mainCamera.setEye(Vector3(0.0f, 0.0f, -50.0f));
    m_mainCamera.setUp();
    m_mainCamera.setFar();
    m_mainCamera.setNear();
    m_mainCamera.setFoV();
    m_mainCamera.setWidth(m_width);
    m_mainCamera.setHeight(m_height);
    m_mainCamera.startCamera();
  }

  /***************************************************************************/
  /**
  * Info passes.
  */
  /***************************************************************************/

  void 
  DeferredRendering::setGBuffer() {
    auto myGraphicsApi = g_graphicApi().instancePtr();

    //We save the input layout.
    myGraphicsApi->setInputLayout(m_pVertexLayout);

    //We save the vertex buffer.
    myGraphicsApi->setVertexBuffer(m_mesh->m_pVertexBuffer);

    //We save the index buffer.
    myGraphicsApi->setIndexBuffer(m_mesh->m_pIndexBuffer);

    //We save the topology.
    myGraphicsApi->setPrimitiveTopology();

    //We keep the cb of the bones.
    myGraphicsApi->setConstBufferBones(m_pCB_BufferBones);
  }

  void
  DeferredRendering::createBlurSSAO() {
    //blurH_Pass(m_pSSAO_RT->getRenderTexture(0));
    //blurV_Pass(m_pSSAO_RT->getRenderTexture(0));
    //
    //additionPass(m_pBlurH_RT->getRenderTexture(0), m_pBlurV_RT->getRenderTexture(0));
    //
    //for (uint32 i = 0; i < 2; ++i) {
    //  blurH_Pass(m_pAddition_RT->getRenderTexture(0));
    //  blurV_Pass(m_pAddition_RT->getRenderTexture(0));
    //  additionPass(m_pBlurH_RT->getRenderTexture(0), m_pBlurV_RT->getRenderTexture(0));
    //}
  }

  void 
  DeferredRendering::createCSBlurSSAO() {
    //csBlurHPass(m_pCompSSAO_RT->getRenderTexture(0));
    //csBlurVPass(m_pCompSSAO_RT->getRenderTexture(0));
    //
    //additionPass(m_pCompBlurH_RT->getRenderTexture(0), m_pCompBlurV_RT->getRenderTexture(0));
    //
    //for (uint32 i = 0; i < 2; ++i) {
    //  csBlurHPass(m_pAddition_RT->getRenderTexture(0));
    //  csBlurVPass(m_pAddition_RT->getRenderTexture(0));
    //  additionPass(m_pCompBlurH_RT->getRenderTexture(0), m_pCompBlurV_RT->getRenderTexture(0));
    //}
  }

  void
  DeferredRendering::createBlurDepth() {
    //csBlurHPass(m_pDepth_RT->getRenderTexture(0));
    //csBlurVPass(m_pDepth_RT->getRenderTexture(0));
    //
    //additionDepthPass(m_pCompBlurH_RT->getRenderTexture(0), m_pCompBlurV_RT->getRenderTexture(0));
    //
    //for (uint32 i = 0; i < 2; ++i) {
    //  csBlurHPass(m_pAdditionShadow_RT->getRenderTexture(0));
    //  csBlurVPass(m_pAdditionShadow_RT->getRenderTexture(0));
    //  additionDepthPass(m_pCompBlurH_RT->getRenderTexture(0), m_pCompBlurV_RT->getRenderTexture(0));
    //}
  }

  /***************************************************************************/
  /**
  * Passes.
  */
  /***************************************************************************/

  void
  DeferredRendering::gbufferPass() {
    auto myGraphicsApi = g_graphicApi().instancePtr();
    auto mySceneGraph = SceneGraph::instancePtr();

    //Here starts the TOTSUGEKI
    myGraphicsApi->setRenderTarget(m_pGbuffer_RT, m_pDepthStencil);
    myGraphicsApi->setShaders(m_pGBuffer_Shader);

    //VS CB
    myGraphicsApi->setYourVSConstantBuffers(m_pCB_BufferCamera, 0);
    myGraphicsApi->setYourVSConstantBuffers(m_pCB_BufferWorld, 1);
    //Animation
    myGraphicsApi->setYourVSConstantBuffers(m_pCB_BufferBones, 2);
    myGraphicsApi->setYourVSConstantBuffers(m_pCB_Shadows, 3);

    //PS CB
    myGraphicsApi->setYourPSConstantBuffers(m_pCB_BufferWorld, 1);
    myGraphicsApi->setYourPSConstantBuffers(m_pCB_Shadows, 3);

    //Clear
    myGraphicsApi->clearYourRenderTarget(m_pGbuffer_RT, m_rgbaCyan);
    myGraphicsApi->clearYourDepthStencilView(m_pDepthStencil);

    cbCamera cameraInfo;
    cameraInfo.mProjection = myGraphicsApi->matrixPolicy(m_mainCamera.getProjection());
    cameraInfo.mView = myGraphicsApi->matrixPolicy(m_mainCamera.getView());

    cbWorld worldInfo;
    worldInfo.mWorld = m_world.identity();
    worldInfo.objectPosition = { 0.0f, 0.0f, 0.0f };

    auto light = m_pLightActor->getComponent<Lights>();
    cbShadows shadowsData;
    shadowsData.mWorld = m_world.identity();
    shadowsData.mView = m_world.identity();
    shadowsData.mProjection = m_world.identity();

    shadowsData.mLightView = myGraphicsApi->matrixPolicy(light->getViewMatrix());
    shadowsData.mLightProjection = myGraphicsApi->matrixPolicy(light->getProjectionMatrix());

    //Update CB.
    myGraphicsApi->updateConstantBuffer(&cameraInfo, m_pCB_BufferCamera);
    myGraphicsApi->updateConstantBuffer(&worldInfo, m_pCB_BufferWorld);
    myGraphicsApi->updateConstantBuffer(&shadowsData, m_pCB_Shadows);

    //Set textures
    myGraphicsApi->setShaderResourceView(m_pDepth_RT->getRenderTexture(0), 6);

    //Render model
    mySceneGraph->render();

    //Des bind
    myGraphicsApi->desbindRT();
    myGraphicsApi->desbindSRV(6);
  }

  void 
  DeferredRendering::SSAO_Pass() {
    auto myGraphicsApi = g_graphicApi().instancePtr();

    //myGraphicsApi->setRenderTarget(m_pSSAO_RT);
    //myGraphicsApi->setShaders(m_pSSAO_Shader);

    //VS CB
    myGraphicsApi->setYourVSConstantBuffers(m_pCB_BufferCamera, 0);
    myGraphicsApi->setYourVSConstantBuffers(m_pCB_BufferWorld, 1);

    //PS CB
    myGraphicsApi->setYourPSConstantBuffers(m_pCB_SSAO, 2);

    //Clear
    //myGraphicsApi->clearYourRenderTarget(m_pSSAO_RT, m_rgbaBlue);
    myGraphicsApi->clearYourDepthStencilView(m_pDepthStencil);

    //Update CB
    //cbSSAO ssaoData;
    //ssaoData.mBias = 0.08000f;
    //ssaoData.mIntensity = 2.0f;
    //ssaoData.mNothing = Vector2(0.0f, 0.0f);
    //ssaoData.mSample_radius = 10.0f;
    //ssaoData.mScale = 1.0f;
    //ssaoData.mViewportDimensions = { (float)m_width, (float)m_height };

    //myGraphicsApi->updateConstantBuffer(&ssaoData, m_pCB_SSAO);

    //Set textures
    myGraphicsApi->setShaderResourceView(m_pGbuffer_RT->getRenderTexture(2), 0);
    myGraphicsApi->setShaderResourceView(m_pGbuffer_RT->getRenderTexture(1), 1);

    m_mySAQ->setSAQ();
  }

  void
  DeferredRendering::CS_SSAO() {
    auto myGraphicsApi = g_graphicApi().instancePtr();

    //Set textures of G buffer to SSAO
    myGraphicsApi->csSetShaderResource(m_pGbuffer_RT->getRenderTexture(2), 0);
    myGraphicsApi->csSetShaderResource(m_pGbuffer_RT->getRenderTexture(1), 1);

    //Set UAV views
    myGraphicsApi->setRtUAV(m_pCompSSAO_RT, 0);

    //Set compute shader
    myGraphicsApi->setComputeShader(m_pCS_SSAO);

    //Update CB
    cbSSAO ssaoInfo;
    ssaoInfo.mViewportDimensions = { (float)m_width, (float)m_height };
    ssaoInfo.mNothing = Vector2(0.0f, 0.0f);

    cbSSAO2 ssaoInfo2;
    ssaoInfo2.mSample_radius = 10.0f;
    ssaoInfo2.mIntensity = 2.0f;
    ssaoInfo2.mScale = 1.0f;
    ssaoInfo2.mBias = 0.08000f;

    myGraphicsApi->updateConstantBuffer(&ssaoInfo, m_pCB_SSAO);
    myGraphicsApi->updateConstantBuffer(&ssaoInfo2, m_pCB_SSAO2);

    //Set constant buffers
    myGraphicsApi->setCSConstantBuffer(m_pCB_SSAO, 0);
    myGraphicsApi->setCSConstantBuffer(m_pCB_SSAO2, 1);

    //Clear RT
    myGraphicsApi->clearYourRenderTarget(m_pCompSSAO_RT, m_rgbaOrange);

    //Dispatch
    myGraphicsApi->dispatch((m_width / 32), (m_height / 32), 1);

    //Des binds
    myGraphicsApi->desbindUAV(0);
    myGraphicsApi->desbindSRV(0);
    myGraphicsApi->desbindSRV(1);
  }

  void
  DeferredRendering::blurH_Pass(void* texture) {
    auto myGraphicsApi = g_graphicApi().instancePtr();

    //myGraphicsApi->setRenderTarget(m_pBlurH_RT);
    //myGraphicsApi->setShaders(m_pBlurH_Shader);

    //VS CB
    myGraphicsApi->setYourVSConstantBuffers(m_pCB_BufferCamera, 0);
    myGraphicsApi->setYourVSConstantBuffers(m_pCB_BufferWorld, 1);

    //PS CB
    myGraphicsApi->setYourPSConstantBuffers(m_pCB_ViewPortDimension, 2);

    //Clear RT
    //myGraphicsApi->clearYourRenderTarget(m_pBlurH_RT, m_rgbaBlue);
    myGraphicsApi->clearYourDepthStencilView(m_pDepthStencil);

    //Update CB
    cbViewportDimension viewportDimenData;
    viewportDimenData.viewportDimensions = { (float)m_width, (float)m_height };
    viewportDimenData.mNothing = { 0.0f, 0.0f };

    myGraphicsApi->updateConstantBuffer(&viewportDimenData, m_pCB_ViewPortDimension);

    //Set texture
    myGraphicsApi->setShaderResourceView(texture, 0);

    m_mySAQ->setSAQ();
  }

  void 
  DeferredRendering::csBlurHPass(void* texture) {
    auto myGraphicsApi = g_graphicApi().instancePtr();

    myGraphicsApi->csSetShaderResource(texture, 0);

    //Set UAV views
    myGraphicsApi->setRtUAV(m_pCompBlurH_RT, 0);

    //Set compute shader
    myGraphicsApi->setComputeShader(m_pCS_BlurH);

    //Update CB
    cbViewportDimension viewportDimenData;
    viewportDimenData.viewportDimensions = { (float)m_width, (float)m_height };
    viewportDimenData.mNothing = { 0.0f, 0.0f };

    myGraphicsApi->updateConstantBuffer(&viewportDimenData, m_pCB_ViewPortDimension);

    //Set constant buffers
    myGraphicsApi->setCSConstantBuffer(m_pCB_ViewPortDimension, 0);

    //Clear RT
    myGraphicsApi->clearYourRenderTarget(m_pCompBlurH_RT, m_rgbaGreen);

    //Dispatch
    myGraphicsApi->dispatch((m_width / 32), (m_height / 32), 1);

    //Des binds
    myGraphicsApi->desbindUAV(0);
    myGraphicsApi->desbindSRV(0);
  }

  void
  DeferredRendering::blurV_Pass(void* texture) {
    auto myGraphicsApi = g_graphicApi().instancePtr();

    //myGraphicsApi->setRenderTarget(m_pBlurV_RT);
    //myGraphicsApi->setShaders(m_pBlurV_Shader);

    //VS CB
    myGraphicsApi->setYourVSConstantBuffers(m_pCB_BufferCamera, 0);
    myGraphicsApi->setYourVSConstantBuffers(m_pCB_BufferWorld, 1);

    //PS CB
    myGraphicsApi->setYourPSConstantBuffers(m_pCB_ViewPortDimension, 2);

    //Clear RT
    //myGraphicsApi->clearYourRenderTarget(m_pBlurV_RT, m_rgbaBlue);
    myGraphicsApi->clearYourDepthStencilView(m_pDepthStencil);

    //Update CB
    cbViewportDimension viewportDimenData;
    viewportDimenData.viewportDimensions = { (float)m_width, (float)m_height };
    viewportDimenData.mNothing = { 0.0f, 0.0f };

    myGraphicsApi->updateConstantBuffer(&viewportDimenData, m_pCB_ViewPortDimension);

    //Set textures
    myGraphicsApi->setShaderResourceView(texture, 0);

    m_mySAQ->setSAQ();
  }

  void 
  DeferredRendering::csBlurVPass(void* texture) {
    auto myGraphicsApi = g_graphicApi().instancePtr();

    myGraphicsApi->csSetShaderResource(texture, 0);

    //Set UAV views
    myGraphicsApi->setRtUAV(m_pCompBlurV_RT, 0);

    //Set compute shader
    myGraphicsApi->setComputeShader(m_pCS_BlurV);

    //Update CB
    cbViewportDimension viewportDimenData;
    viewportDimenData.viewportDimensions = { (float)m_width, (float)m_height };
    viewportDimenData.mNothing = { 0.0f, 0.0f };

    myGraphicsApi->updateConstantBuffer(&viewportDimenData, m_pCB_ViewPortDimension);

    //Set constant buffers
    myGraphicsApi->setCSConstantBuffer(m_pCB_ViewPortDimension, 0);

    //Clear RT
    myGraphicsApi->clearYourRenderTarget(m_pCompBlurV_RT, m_rgbaBlue);

    //Dispatch
    myGraphicsApi->dispatch((m_width / 32), (m_height / 32), 1);

    //Des binds
    myGraphicsApi->desbindUAV(0);
    myGraphicsApi->desbindSRV(0);
  }

  void
  DeferredRendering::additionPass(void* texture1, void* texture2) {
    //auto myGraphicsApi = g_graphicApi().instancePtr();
    //
    ////Des bind
    //myGraphicsApi->desbindRT();
    //myGraphicsApi->desbindSRV(0);
    //myGraphicsApi->desbindSRV(1);
    //
    //myGraphicsApi->setRenderTarget(m_pAddition_RT);
    //myGraphicsApi->setShaders(m_pAddition_Shader);
    //
    ////VS CB
    //myGraphicsApi->setYourVSConstantBuffers(m_pCB_BufferCamera, 0);
    //myGraphicsApi->setYourVSConstantBuffers(m_pCB_BufferWorld, 1);
    //
    ////PS CB
    //myGraphicsApi->setYourPSConstantBuffers(m_pCB_MipLevels, 2);
    //
    ////Clean
    //myGraphicsApi->clearYourRenderTarget(m_pAddition_RT, m_rgbaGreen);
    //myGraphicsApi->clearYourDepthStencilView(m_pDepthStencil);
    //
    ////Update CB
    //cbMipLevels mipLevelsData;
    //mipLevelsData.mipLevel0 = 1;
    //mipLevelsData.mipLevel1 = 1;
    //mipLevelsData.mipLevel2 = 1;
    //mipLevelsData.mipLevel3 = 1;
    //
    //myGraphicsApi->updateConstantBuffer(&mipLevelsData, m_pCB_MipLevels);
    //
    ////Set textures
    //myGraphicsApi->setShaderResourceView(texture1, 0);
    //myGraphicsApi->setShaderResourceView(texture2, 1);
    //
    //m_mySAQ->setSAQ();
    //
    ////Des bind
    //myGraphicsApi->desbindRT();
    //myGraphicsApi->desbindSRV(0);
    //myGraphicsApi->desbindSRV(1);
  }

  void 
  DeferredRendering::additionDepthPass(void* texture1, void* texture2) {
    //auto myGraphicsApi = g_graphicApi().instancePtr();
    //
    ////Des bind
    //myGraphicsApi->desbindRT();
    //myGraphicsApi->desbindSRV(0);
    //myGraphicsApi->desbindSRV(1);
    //
    //myGraphicsApi->setRenderTarget(m_pAdditionShadow_RT);
    //myGraphicsApi->setShaders(m_pAdditionDepth_Shader);
    //
    ////VS CB
    //myGraphicsApi->setYourVSConstantBuffers(m_pCB_BufferCamera, 0);
    //myGraphicsApi->setYourVSConstantBuffers(m_pCB_BufferWorld, 1);
    //
    ////PS CB
    //myGraphicsApi->setYourPSConstantBuffers(m_pCB_MipLevels, 2);
    //
    ////Clear
    //myGraphicsApi->clearYourRenderTarget(m_pAdditionShadow_RT, m_rgbaBlue);
    //myGraphicsApi->clearYourDepthStencilView(m_pDepthStencil);
    //
    ////Update CB
    //cbMipLevels mipLevelsData;
    //mipLevelsData.mipLevel0 = 1;
    //mipLevelsData.mipLevel1 = 1;
    //mipLevelsData.mipLevel2 = 1;
    //mipLevelsData.mipLevel3 = 1;
    //
    //myGraphicsApi->updateConstantBuffer(&mipLevelsData, m_pCB_MipLevels);
    //
    ////Set textures
    //myGraphicsApi->setShaderResourceView(texture1, 0);
    //myGraphicsApi->setShaderResourceView(texture2, 1);
    //
    //m_mySAQ->setSAQ();
    //
    ////Des bind
    //myGraphicsApi->desbindRT();
    //myGraphicsApi->desbindSRV(0);
    //myGraphicsApi->desbindSRV(1);
  }

  void
  DeferredRendering::lightningPass() {
    auto myGraphicsApi = g_graphicApi().instancePtr();

    myGraphicsApi->setRenderTarget(m_pRenderTargetView);
    myGraphicsApi->setShaders(m_pLightning_Shader);

    //VS CB
    myGraphicsApi->setYourVSConstantBuffers(m_pCB_BufferCamera, 0);
    myGraphicsApi->setYourVSConstantBuffers(m_pCB_BufferWorld, 1);

    //PS CB
    myGraphicsApi->setYourPSConstantBuffers(m_pCB_BufferCamera, 0);
    myGraphicsApi->setYourPSConstantBuffers(m_pCB_BufferWorld, 1);
    myGraphicsApi->setYourPSConstantBuffers(m_pCB_Lightning, 2);
    myGraphicsApi->setYourPSConstantBuffers(m_pCB_InverseMat, 3);

    //Clean RT
    myGraphicsApi->clearYourRenderTargetView(m_pRenderTargetView, m_rgbaYellow);
    myGraphicsApi->clearYourDepthStencilView(m_pDepthStencil);

    auto light = m_pLightActor->getComponent<Lights>();

    //Update CB
    cbLightning lightningData;
    lightningData.emissiveIntensity = light->getEmissiveIntensity();
    lightningData.lightIntensity = light->getIntensity();

    lightningData.lightPosX = light->getPosition().x;
    lightningData.lightPosY = light->getPosition().y;
    lightningData.lightPosZ = light->getPosition().z;

    //lightningData.vViewPositionX = m_mainCamera.getCamEye().x;
    //lightningData.vViewPositionY = m_mainCamera.getCamEye().y;
    //lightningData.vViewPositionZ = m_mainCamera.getCamEye().z;

    lightningData.vViewPositionX = light->getLookAt().x;
    lightningData.vViewPositionY = light->getLookAt().y;
    lightningData.vViewPositionZ = light->getLookAt().z;

    cbInverse inverseData;
    inverseData.mInverseViewCam = myGraphicsApi->matrixPolicy(m_mainCamera.getView().invert());
    inverseData.mInverseProjectionCam = myGraphicsApi->matrixPolicy(m_mainCamera.getProjection().invert());

    myGraphicsApi->updateConstantBuffer(&lightningData, m_pCB_Lightning);
    myGraphicsApi->updateConstantBuffer(&inverseData, m_pCB_InverseMat);

    //Set textures
    myGraphicsApi->setShaderResourceView(m_pGbuffer_RT->getRenderTexture(0), 0);
    myGraphicsApi->setShaderResourceView(m_pGbuffer_RT->getRenderTexture(1), 1);
    myGraphicsApi->setShaderResourceView(m_pGbuffer_RT->getRenderTexture(3), 2);
    myGraphicsApi->setShaderResourceView(m_pCompBlurV_RT->getRenderTexture(0), 3);
    myGraphicsApi->setShaderResourceView(m_pGbuffer_RT->getRenderTexture(2), 4);

    m_mySAQ->setSAQ();

    myGraphicsApi->desbindSRV(0);
    myGraphicsApi->desbindSRV(1);
    myGraphicsApi->desbindSRV(2);
    myGraphicsApi->desbindSRV(3);
    myGraphicsApi->desbindSRV(4);
  }

  void 
  DeferredRendering::depthPass() {
    auto myGraphicsApi = g_graphicApi().instancePtr();
    auto mySceneGraph = SceneGraph::instancePtr();

    //We save the input layout.
    myGraphicsApi->setInputLayout(m_pDepthLayout);

    myGraphicsApi->setRenderTarget(m_pDepth_RT);
    myGraphicsApi->setShaders(m_pDepth_Shader);

    //VS CB
    myGraphicsApi->setYourVSConstantBuffers(m_pCB_Depth, 0);

    //Animation
    myGraphicsApi->setYourVSConstantBuffers(m_pCB_BufferBones, 1);

    //Clear m_rgbaGray
    myGraphicsApi->clearYourRenderTarget(m_pDepth_RT, m_rgbaMagenta);
    myGraphicsApi->clearYourDepthStencilView(m_pDepth_RT);

    auto light = m_pLightActor->getComponent<Lights>();
    //Update CB
    cbMatrixBuffer matrixData;
    matrixData.mProjection = myGraphicsApi->matrixPolicy(light->getProjectionMatrix());
    matrixData.mView = myGraphicsApi->matrixPolicy(light->getViewMatrix());
    matrixData.mWorld = m_world.identity();

    //Update CB.
    myGraphicsApi->updateConstantBuffer(&matrixData, m_pCB_Depth);

    //Render model
    mySceneGraph->render();

    //Des bind
    myGraphicsApi->desbindRT();
  }

  /***************************************************************************/
  /**
  * .
  */
  /***************************************************************************/

  void
  DeferredRendering::defaultLight() {
    SPtr<Lights> lightCompo = make_shared<Lights>(Vector3(0.0f, 80.0f, -68.0f),
                                                  Vector3(0.0f, 80.0f, 0.0f));

    //Light info
    lightCompo->setAmbientColor(0.15f, 0.15f, 0.15f, 1.0f);
    lightCompo->setDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);

    lightCompo->setIntensity(2.0f);
    lightCompo->setEmissiveIntensity(1.5f);

    auto myRSRCMG = ResourceManager::instancePtr();
    auto mySceneGraph = SceneGraph::instancePtr();

    SPtr<Models> myModel = myRSRCMG->load<Models>("data/models/basicModels/sphere.fbx");

    SPtr<StaticMesh> myStaticMesh = make_shared<StaticMesh>();
    myStaticMesh->m_pModel = myModel;

    //Creating actor
    m_pLightActor.reset(new Actor("LightActor"));
    m_pLightActor->setIsSelected(true);
    m_pLightActor->setComponent(lightCompo);
    m_pLightActor->setComponent(myStaticMesh);

    auto transform = m_pLightActor->getComponent<Transform>();
    transform->setScale(Vector3(5.0f, 5.0f, 5.0f));
    transform->setPosition(Vector3(0.0f, 80.0f, -68.0f));

    mySceneGraph->createNewActor(m_pLightActor, SPtr<SceneNode>(nullptr));
  }

  void
  DeferredRendering::createSamplers() {
    auto myGraphicsApi = g_graphicApi().instancePtr();

    m_pSampler = myGraphicsApi->createSamplerState();

    m_pSampleStateClamp = myGraphicsApi->createSamplerState(
                                         FILTER::kFilterMinMagMipLinear,
                                         TEXTURE_ADDRESS::kTextureAddressClamp,
                                         COMPARISON::kComparisonAlways);

    myGraphicsApi->setSamplerState(m_pSampler, 0);
    myGraphicsApi->setSamplerState(m_pSampleStateClamp, 1);

    m_pCSSampler = myGraphicsApi->createSamplerState();
    //Set sampler
    myGraphicsApi->setCSSampler(m_pCSSampler, 0);
  }

  void
  DeferredRendering::createRenderTargets() {
    auto myGraphicsApi = g_graphicApi().instancePtr();

    m_pGbuffer_RT        = make_shared<RenderTarget>();
    //m_pSSAO_RT           = make_shared<RenderTarget>();
    //m_pBlurH_RT          = make_shared<RenderTarget>();
    //m_pBlurV_RT          = make_shared<RenderTarget>();
    //m_pAddition_RT       = make_shared<RenderTarget>();
    //m_pAdditionShadow_RT = make_shared<RenderTarget>();
    m_pLightning_RT      = make_shared<RenderTarget>();

    //Depth
    m_pDepth_RT = make_shared<RenderTarget>();

    //Compute
    m_pCompSSAO_RT = make_shared<RenderTarget>();

    m_pGbuffer_RT        = myGraphicsApi->createRenderTarget(m_width, m_height, 1, 4);
    //m_pSSAO_RT           = myGraphicsApi->createRenderTarget(m_width, m_height);
    //m_pBlurH_RT          = myGraphicsApi->createRenderTarget(m_width, m_height);
    //m_pBlurV_RT          = myGraphicsApi->createRenderTarget(m_width, m_height);
    //m_pAddition_RT       = myGraphicsApi->createRenderTarget(m_width, m_height);
    //m_pAdditionShadow_RT = myGraphicsApi->createRenderTarget(m_width, m_height);
    m_pLightning_RT      = myGraphicsApi->createRenderTarget(m_width, m_height);

    //Depth
    m_pDepth_RT = myGraphicsApi->createRenderTarget(m_width,
                                                    m_height, 
                                                    1,
                                                    1,
                                                    1.0f,
                                                    true,
                                                    TEXTURE_FORMAT::E::kR16Float);
    //Compute
    m_pCompSSAO_RT = myGraphicsApi->createRenderTarget(
                                    m_width,
                                    m_height, 
                                    1,
                                    1,
                                    1.0f,
                                    false,
                                    TEXTURE_FORMAT::E::kR32G32B32A32Float,
                                    TEXTURE_BIND_FLAGS::kBindUnorderedAccess);

    m_pCompBlurH_RT = myGraphicsApi->createRenderTarget(
                                     m_width,
                                     m_height, 
                                     1,
                                     1,
                                     1.0f,
                                     false,
                                     TEXTURE_FORMAT::E::kR32G32B32A32Float,
                                     TEXTURE_BIND_FLAGS::kBindUnorderedAccess);

    m_pCompBlurV_RT = myGraphicsApi->createRenderTarget(
                                     m_width,
                                     m_height, 
                                     1,
                                     1,
                                     1.0f,
                                     false,
                                     TEXTURE_FORMAT::E::kR32G32B32A32Float,
                                     TEXTURE_BIND_FLAGS::kBindUnorderedAccess);
  }

  void
  DeferredRendering::createBuffers() {
    auto myGraphicsApi = g_graphicApi().instancePtr();

    m_pCB_BufferCamera.reset(myGraphicsApi->createConstantBuffer(sizeof(cbCamera)));
    m_pCB_BufferWorld.reset(myGraphicsApi->createConstantBuffer(sizeof(cbWorld)));
    m_pCB_BufferBones.reset(myGraphicsApi->createConstantBuffer(sizeof(cbModelData)));
    m_pCB_SSAO.reset(myGraphicsApi->createConstantBuffer(sizeof(cbSSAO)));
    m_pCB_SSAO2.reset(myGraphicsApi->createConstantBuffer(sizeof(cbSSAO2)));
    m_pCB_ViewPortDimension.reset(myGraphicsApi->createConstantBuffer(sizeof(cbViewportDimension)));
    m_pCB_Lightning.reset(myGraphicsApi->createConstantBuffer(sizeof(cbLightning)));
    m_pCB_MipLevels.reset(myGraphicsApi->createConstantBuffer(sizeof(cbMipLevels)));

    //Depth
    m_pCB_Depth.reset(myGraphicsApi->createConstantBuffer(sizeof(cbMatrixBuffer)));
    m_pCB_Shadows.reset(myGraphicsApi->createConstantBuffer(sizeof(cbShadows)));

    //Inverse
    m_pCB_InverseMat.reset(myGraphicsApi->createConstantBuffer(sizeof(cbInverse)));
  }

  void
  DeferredRendering::loadShadersFiles() {
    auto myGraphicsApi = g_graphicApi().instancePtr();

    //We create the vertex shader and pixel shader.
    m_pGBuffer_Shader.reset(myGraphicsApi->createShadersProgram(
                                           L"data/shaders/DX_gBuffer.hlsl",
                                           "vs_gBuffer",
                                           L"data/shaders/DX_gBuffer.hlsl",
                                           "ps_gBuffer"));

    //m_pSSAO_Shader.reset(myGraphicsApi->createShadersProgram(
    //                                    L"data/shaders/DX_screenAlignedQuad.hlsl",
    //                                    "vs_ssAligned",
    //                                    L"data/shaders/DX_SSAO.hlsl",
    //                                    "ps_ssao"));
    //
    //m_pBlurH_Shader.reset(myGraphicsApi->createShadersProgram(
    //                                     L"data/shaders/DX_screenAlignedQuad.hlsl",
    //                                     "vs_ssAligned",
    //                                     L"data/shaders/DX_gaussyan_blur.hlsl",
    //                                     "ps_gaussian_blurH"));
    //
    //m_pBlurV_Shader.reset(myGraphicsApi->createShadersProgram(
    //                                     L"data/shaders/DX_screenAlignedQuad.hlsl",
    //                                     "vs_ssAligned",
    //                                     L"data/shaders/DX_gaussyan_blur.hlsl",
    //                                     "ps_gaussian_blurV"));

    m_pLightning_Shader.reset(myGraphicsApi->createShadersProgram(
                                             L"data/shaders/DX_screenAlignedQuad.hlsl",
                                             "vs_ssAligned",
                                             L"data/shaders/DX_lightningPS.hlsl",
                                             "ps_main"));
    //m_pAdditionDepth_Shader.reset(myGraphicsApi->createShadersProgram(
    //                                             L"data/shaders/DX_screenAlignedQuad.hlsl",
    //                                             "vs_ssAligned",
    //                                             L"data/shaders/DX_AdditionPS.hlsl",
    //                                             "Add"));
    m_pDepth_Shader.reset(myGraphicsApi->createShadersProgram(L"data/shaders/DX_Depth.hlsl",
                                                              "DepthVS",
                                                              L"data/shaders/DX_Depth.hlsl",
                                                              "DepthPS"));
    /*
    * C O M P U T E
    * S H A D E R
    */

    //Ambient occlusion
    m_pCS_SSAO.reset(myGraphicsApi->createComputeShaderProgram(
                                    L"data/shaders/computeShaders/CS_SSAO.hlsl",
                                    "cs_SSAO"));

    //Blurs
    m_pCS_BlurH.reset(myGraphicsApi->createComputeShaderProgram(
                                     L"data/shaders/computeShaders/CS_GaussianBlurs.hlsl",
                                     "cs_gaussian_blurH"));
    m_pCS_BlurV.reset(myGraphicsApi->createComputeShaderProgram(
                                     L"data/shaders/computeShaders/CS_GaussianBlurs.hlsl",
                                     "cs_gaussian_blurV"));
  }
}