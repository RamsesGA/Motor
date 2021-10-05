#include <gaGraphicsApi.h>
#include <gaPlane.h>
#include <gaRenderTarget.h>
#include <gaBaseInterface.h>

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

    //We create the vertex shader and pixel shader.
    m_pGBuffer_Shader.reset(myGraphicsApi->createShadersProgram(
                                           L"data/shaders/DX_gBuffer.hlsl",
                                           "vs_gBuffer",
                                           L"data/shaders/DX_gBuffer.hlsl",
                                           "ps_gBuffer"));

    m_pSSAO_Shader.reset(myGraphicsApi->createShadersProgram(
                                        L"data/shaders/DX_screenAlignedQuad.hlsl",
                                        "vs_ssAligned",
                                        L"data/shaders/DX_SSAO.hlsl",
                                        "ps_ssao"));

    m_pBlurH_Shader.reset(myGraphicsApi->createShadersProgram(
                                         L"data/shaders/DX_screenAlignedQuad.hlsl",
                                         "vs_ssAligned",
                                         L"data/shaders/DX_gaussyan_blur.hlsl",
                                         "ps_gaussian_blurH"));

    m_pBlurV_Shader.reset(myGraphicsApi->createShadersProgram(
                                         L"data/shaders/DX_screenAlignedQuad.hlsl",
                                         "vs_ssAligned",
                                         L"data/shaders/DX_gaussyan_blur.hlsl",
                                         "ps_gaussian_blurV"));

    m_pLightning_Shader.reset(myGraphicsApi->createShadersProgram(
                                             L"data/shaders/DX_screenAlignedQuad.hlsl",
                                             "vs_ssAligned",
                                             L"data/shaders/DX_lightningPS.hlsl",
                                             "ps_main"));

    m_pAddition_Shader.reset(myGraphicsApi->createShadersProgram( 
                                            L"data/shaders/DX_screenAlignedQuad.hlsl",
                                            "vs_ssAligned",
                                            L"data/shaders/DX_AdditionPS.hlsl",
                                            "Add"));

    m_pAdditionShadow_Shader = m_pAddition_Shader;

    m_pDepth_Shader.reset(myGraphicsApi->createShadersProgram(L"data/shaders/DX_Depth.hlsl",
                                                              "DepthVS",
                                                              L"data/shaders/DX_Depth.hlsl",
                                                              "DepthPS"));

    m_pShadowMap_Shader.reset(myGraphicsApi->createShadersProgram(
                                             L"data/shaders/DX_ShadowMap.hlsl",
                                             "ShadowVS",
                                             L"data/shaders/DX_ShadowMap.hlsl",
                                             "ShadowPS"));

    /*
    * C R E A T E
    * I N P U T
    * L A Y O U T
    * Z O N E
    */
    m_pVertexLayout.reset(myGraphicsApi->createInputLayout(m_pGBuffer_Shader));

    m_pDepthLayout.reset(myGraphicsApi->createInputLayout(m_pDepth_Shader));

    //We create the vertex buffer.
    m_mesh->m_pVertexBuffer.reset(myGraphicsApi->createVertexBuffer(nullptr, sizeof(Matrices)));

    //We create the index buffer.
    m_mesh->m_pIndexBuffer.reset(myGraphicsApi->createIndexBuffer(nullptr, sizeof(ViewCB)));

    /*
    * C R E A T E
    * B U F F E R S
    * Z O N E
    */
    m_pCB_BufferCamera.reset(myGraphicsApi->createConstantBuffer(sizeof(cbCamera)));
    m_pCB_BufferWorld.reset(myGraphicsApi->createConstantBuffer(sizeof(cbWorld)));
    m_pCB_BufferBones.reset(myGraphicsApi->createConstantBuffer(sizeof(cbBonesTransform)));
    m_pCB_SSAO.reset(myGraphicsApi->createConstantBuffer(sizeof(cbSSAO)));
    m_pCB_ViewPortDimension.reset(myGraphicsApi->createConstantBuffer(sizeof(cbViewportDimension)));
    m_pCB_Lightning.reset(myGraphicsApi->createConstantBuffer(sizeof(cbLightning)));
    m_pCB_MipLevels.reset(myGraphicsApi->createConstantBuffer(sizeof(cbMipLevels)));

    //Depth
    m_pCB_Depth.reset(myGraphicsApi->createConstantBuffer(sizeof(cbMatrixBuffer)));

    //Shadow map
    m_pCB_Shadows.reset(myGraphicsApi->createConstantBuffer(sizeof(cbShadows)));

    m_pCB_Light.reset(myGraphicsApi->createConstantBuffer(sizeof(cbLight)));

    m_pCB_Light2.reset(myGraphicsApi->createConstantBuffer(sizeof(cbLight2)));

    //Inverse
    m_pCB_InverseMat.reset(myGraphicsApi->createConstantBuffer(sizeof(cbInverse)));

    /*
    * C R E A T E
    * R E N D E R
    * T A R G E TS
    * Z O N E
    */
    m_pGbuffer_RT        = make_shared<RenderTarget>();
    m_pSSAO_RT           = make_shared<RenderTarget>();
    m_pBlurH_RT          = make_shared<RenderTarget>();
    m_pBlurV_RT          = make_shared<RenderTarget>();
    m_pAddition_RT       = make_shared<RenderTarget>();
    m_pAdditionShadow_RT = make_shared<RenderTarget>();
    m_pLightning_RT      = make_shared<RenderTarget>();

    //Depth
    m_pDepth_RT = make_shared<RenderTarget>();

    //Shadow map
    m_pShadowMap_RT = make_shared<RenderTarget>();

    m_pGbuffer_RT        = myGraphicsApi->createRenderTarget(m_width, m_height, 1, 4);
    m_pSSAO_RT           = myGraphicsApi->createRenderTarget(m_width, m_height);
    m_pBlurH_RT          = myGraphicsApi->createRenderTarget(m_width, m_height);
    m_pBlurV_RT          = myGraphicsApi->createRenderTarget(m_width, m_height);
    m_pAddition_RT       = myGraphicsApi->createRenderTarget(m_width, m_height);
    m_pAdditionShadow_RT = myGraphicsApi->createRenderTarget(m_width, m_height);
    m_pLightning_RT      = myGraphicsApi->createRenderTarget(m_width, m_height);

    //Depth
    m_pDepth_RT = myGraphicsApi->createRenderTarget(m_width,
                                                    m_height, 
                                                    1,
                                                    1,
                                                    1.0f,
                                                    true,
                                                    TEXTURE_FORMAT::E::kR16Float);

    //Shadow map
    m_pShadowMap_RT = myGraphicsApi->createRenderTarget(m_width, m_height, 1, 1, 1.0f, true);

    /*
    * C R E A T E
    * S A M P L E R
    * Z O N E
    */
    m_pSampler = myGraphicsApi->createSamplerState();

    m_pSampleStateClamp = myGraphicsApi->createSamplerState(
                                         FILTER::kFilterMinMagMipLinear,
                                         TEXTURE_ADDRESS::kTextureAddressClamp,
                                         COMPARISON::kComparisonAlways);

    m_pSampleStateWrap = myGraphicsApi->createSamplerState();

    myGraphicsApi->setSamplerState(m_pSampler, 0);
    myGraphicsApi->setSamplerState(m_pSampleStateClamp, 1);
    myGraphicsApi->setSamplerState(m_pSampleStateWrap, 2);

    //Create the one face of cube.
    m_mySAQ.reset(new Plane());
    m_mySAQ->createSAQ();

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
    m_pLight = make_shared<Lights>();

    //Light info
    m_pLight->setAmbientColor(0.15f, 0.15f, 0.15f, 1.0f);
    m_pLight->setDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);

    m_pLight->setLookAt(m_shadowCamera.getLookAt().x,
                        m_shadowCamera.getLookAt().y,
                        m_shadowCamera.getLookAt().z);

    m_pLight->setPosition(m_shadowCamera.getCamEye().x, 
                          m_shadowCamera.getCamEye().y, 
                          m_shadowCamera.getCamEye().z);

    m_pLight->setIntensity(2.0f);
    m_pLight->setEmissiveIntensity(1.5f);
  }

  void
  DeferredRendering::update(const float& deltaTime) {
    auto myGraphicsApi = g_graphicApi().instancePtr();

    /*
    * I N P U T S
    */
    keyboardButtons(deltaTime);
    mouseRotation(deltaTime);

    /*
    * P A S S E S
    * Z O N E
    */
    depthPass();

    shadowMapPass();

    gbufferPass();
    SSAO_Pass();

    //Blur AO
    createBlurSSAO();
    
    //Blur Shadow
    createBlurShadow();

    //Final pass
    lightningPass();

    // Clear the depth buffer to 1.0 (max depth).
    myGraphicsApi->clearYourDepthStencilView(m_pDepthStencil);
  }

  void
  DeferredRendering::render() {
    auto myGraphicsApi = g_graphicApi().instancePtr();

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

    /*
    * C A M E R A
    * S H A D O W
    * I N F O
    */
    Vector4 eye    = Vector4(1.78f, -115.56f, 206.12f, 1.0f);
    Vector4 lookAt = Vector4(0.0f,   0.0f,  0.0f,   0.0f);
    Vector4 up     = Vector4(0.0f,   1.0f,  0.0f,   0.0f);

    Matrix4x4 mView = { -1.0f, -0.01f, 0.01f, 0.0f, 
                        -0.0f, -0.16f, -0.99f, 0.0f,
                        0.01f, -0.99f, 0.16f, 0.0f,
                        -19.41f, -81.41f, 330.27f, 1.0f, };

    m_shadowCamera.setLookAt(Vector3(0.0f, 0.0f, 0.0f));
    m_shadowCamera.setEye(Vector3(1.78f, -115.56f, 206.12f));
    m_shadowCamera.setUp();
    m_shadowCamera.setFar();
    m_shadowCamera.setNear();
    m_shadowCamera.setFoV();
    m_shadowCamera.setWidth(m_width);
    m_shadowCamera.setHeight(m_height);
    m_shadowCamera.startCamera();

    m_shadowCamera.setView(mView);
    m_shadowCamera.setProjection(createOrtographicProyectionLH(-500.0f,
                                                               500.0f,
                                                               -500.0f,
                                                               500.0f,
                                                               0.1f,
                                                               2000.0f));
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
    blurH_Pass(m_pSSAO_RT->getRenderTexture(0));
    blurV_Pass(m_pSSAO_RT->getRenderTexture(0));

    additionPass(m_pBlurH_RT->getRenderTexture(0), m_pBlurV_RT->getRenderTexture(0));

    for (uint32 i = 0; i < 2; ++i) {
      blurH_Pass(m_pAddition_RT->getRenderTexture(0));
      blurV_Pass(m_pAddition_RT->getRenderTexture(0));
      additionPass(m_pBlurH_RT->getRenderTexture(0), m_pBlurV_RT->getRenderTexture(0));
    }
  }

  void
  DeferredRendering::createBlurShadow() {
    blurH_Pass(m_pDepth_RT->getRenderTexture(0));
    blurV_Pass(m_pDepth_RT->getRenderTexture(0));

    additionShadowPass(m_pBlurH_RT->getRenderTexture(0), m_pBlurV_RT->getRenderTexture(0));

    for (uint32 i = 0; i < 2; ++i) {
      blurH_Pass(m_pAdditionShadow_RT->getRenderTexture(0));
      blurV_Pass(m_pAdditionShadow_RT->getRenderTexture(0));
      additionShadowPass(m_pBlurH_RT->getRenderTexture(0), m_pBlurV_RT->getRenderTexture(0));
    }
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
    //myGraphicsApi->setYourVSConstantBuffers(m_pCBufferBones, 2);
    //PS CB
    //myGraphicsApi->setYourPSConstantBuffers(m_pCB_BufferWorld, 1);

    //Clear
    myGraphicsApi->clearYourRenderTarget(m_pGbuffer_RT, m_rgbaBlue);
    myGraphicsApi->clearYourDepthStencilView(m_pDepthStencil);

    cbCamera cameraInfo;
    cameraInfo.mProjection = myGraphicsApi->matrixPolicy(m_mainCamera.getProjection());
    cameraInfo.mView = myGraphicsApi->matrixPolicy(m_mainCamera.getView());

    cbWorld worldInfo;
    worldInfo.mWorld = m_world.identity();
    worldInfo.objectPosition = { 0.0f, 0.0f, 0.0f };

    //Update CB.
    myGraphicsApi->updateConstantBuffer(&cameraInfo, m_pCB_BufferCamera);
    myGraphicsApi->updateConstantBuffer(&worldInfo, m_pCB_BufferWorld);

    //Render model
    mySceneGraph->render();
  }

  void 
  DeferredRendering::SSAO_Pass() {
    auto myGraphicsApi = g_graphicApi().instancePtr();

    myGraphicsApi->setRenderTarget(m_pSSAO_RT);
    myGraphicsApi->setShaders(m_pSSAO_Shader);

    //VS CB
    myGraphicsApi->setYourVSConstantBuffers(m_pCB_BufferCamera, 0);
    myGraphicsApi->setYourVSConstantBuffers(m_pCB_BufferWorld, 1);

    //PS CB
    myGraphicsApi->setYourPSConstantBuffers(m_pCB_SSAO, 2);

    //Clear
    myGraphicsApi->clearYourRenderTarget(m_pSSAO_RT, m_rgbaBlue);
    myGraphicsApi->clearYourDepthStencilView(m_pDepthStencil);

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
    myGraphicsApi->setShaderResourceView(m_pGbuffer_RT->getRenderTexture(2), 0);
    myGraphicsApi->setShaderResourceView(m_pGbuffer_RT->getRenderTexture(1), 1);

    m_mySAQ->setSAQ();
  }

  void 
  DeferredRendering::blurH_Pass(void* texture) {
    auto myGraphicsApi = g_graphicApi().instancePtr();

    myGraphicsApi->setRenderTarget(m_pBlurH_RT);
    myGraphicsApi->setShaders(m_pBlurH_Shader);

    //VS CB
    myGraphicsApi->setYourVSConstantBuffers(m_pCB_BufferCamera, 0);
    myGraphicsApi->setYourVSConstantBuffers(m_pCB_BufferWorld, 1);

    //PS CB
    myGraphicsApi->setYourPSConstantBuffers(m_pCB_ViewPortDimension, 2);

    //Clear RT
    myGraphicsApi->clearYourRenderTarget(m_pBlurH_RT, m_rgbaBlue);
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
  DeferredRendering::blurV_Pass(void* texture) {
    auto myGraphicsApi = g_graphicApi().instancePtr();

    myGraphicsApi->setRenderTarget(m_pBlurV_RT);
    myGraphicsApi->setShaders(m_pBlurV_Shader);

    //VS CB
    myGraphicsApi->setYourVSConstantBuffers(m_pCB_BufferCamera, 0);
    myGraphicsApi->setYourVSConstantBuffers(m_pCB_BufferWorld, 1);

    //PS CB
    myGraphicsApi->setYourPSConstantBuffers(m_pCB_ViewPortDimension, 2);

    //Clear RT
    myGraphicsApi->clearYourRenderTarget(m_pBlurV_RT, m_rgbaBlue);
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
  DeferredRendering::additionPass(void* texture1, void* texture2) {
    auto myGraphicsApi = g_graphicApi().instancePtr();

    myGraphicsApi->setRenderTarget(m_pAddition_RT);
    myGraphicsApi->setShaders(m_pAddition_Shader);

    //VS CB
    myGraphicsApi->setYourVSConstantBuffers(m_pCB_BufferCamera, 0);
    myGraphicsApi->setYourVSConstantBuffers(m_pCB_BufferWorld, 1);

    //PS CB
    myGraphicsApi->setYourPSConstantBuffers(m_pCB_MipLevels, 2);

    //Clean
    myGraphicsApi->clearYourRenderTarget(m_pAddition_RT, m_rgbaBlue);
    myGraphicsApi->clearYourDepthStencilView(m_pDepthStencil);

    //Update CB
    cbMipLevels mipLevelsData;
    mipLevelsData.mipLevel0 = 1;
    mipLevelsData.mipLevel1 = 1;
    mipLevelsData.mipLevel2 = 1;
    mipLevelsData.mipLevel3 = 1;

    myGraphicsApi->updateConstantBuffer(&mipLevelsData, m_pCB_MipLevels);

    //Set textures
    myGraphicsApi->setShaderResourceView(texture1, 0);
    myGraphicsApi->setShaderResourceView(texture2, 1);

    m_mySAQ->setSAQ();
  }

  void 
  DeferredRendering::additionShadowPass(void* texture1, void* texture2) {
    auto myGraphicsApi = g_graphicApi().instancePtr();

    myGraphicsApi->setRenderTarget(m_pAdditionShadow_RT);
    myGraphicsApi->setShaders(m_pAdditionShadow_Shader);

    //VS CB
    myGraphicsApi->setYourVSConstantBuffers(m_pCB_BufferCamera, 0);
    myGraphicsApi->setYourVSConstantBuffers(m_pCB_BufferWorld, 1);

    //PS CB
    myGraphicsApi->setYourPSConstantBuffers(m_pCB_MipLevels, 2);

    //Clear
    myGraphicsApi->clearYourRenderTarget(m_pAdditionShadow_RT, m_rgbaBlue);
    myGraphicsApi->clearYourDepthStencilView(m_pDepthStencil);

    //Update CB
    cbMipLevels mipLevelsData;
    mipLevelsData.mipLevel0 = 1;
    mipLevelsData.mipLevel1 = 1;
    mipLevelsData.mipLevel2 = 1;
    mipLevelsData.mipLevel3 = 1;

    myGraphicsApi->updateConstantBuffer(&mipLevelsData, m_pCB_MipLevels);

    //Set textures
    myGraphicsApi->setShaderResourceView(texture1, 0);
    myGraphicsApi->setShaderResourceView(texture2, 1);

    m_mySAQ->setSAQ();
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
    myGraphicsApi->setYourPSConstantBuffers(m_pCB_Shadows, 4);

    //Clean RT
    myGraphicsApi->clearYourRenderTargetView(m_pRenderTargetView, m_rgbaBlue);
    myGraphicsApi->clearYourDepthStencilView(m_pDepthStencil);

    //Update CB
    cbLightning lightningData;
    lightningData.emissiveIntensity = m_pLight->getEmissiveIntensity();
    lightningData.lightIntensity = m_pLight->getIntensity();

    lightningData.lightPosX = m_shadowCamera.getCamEye().x;
    lightningData.lightPosY = m_shadowCamera.getCamEye().y;
    lightningData.lightPosZ = m_shadowCamera.getCamEye().z;

    lightningData.vViewPositionX = m_shadowCamera.getLookAt().x;
    lightningData.vViewPositionY = m_shadowCamera.getLookAt().y;
    lightningData.vViewPositionZ = m_shadowCamera.getLookAt().z;

    cbInverse inverseData;
    inverseData.mInverseViewCam = myGraphicsApi->matrixPolicy(m_mainCamera.getView().invert());
    inverseData.mInverseProjectionCam = myGraphicsApi->matrixPolicy(m_mainCamera.getProjection().invert());

    cbShadows shadowsData;
    shadowsData.mWorld = m_world.identity();
    shadowsData.mView = m_world.identity();
    shadowsData.mProjection = m_world.identity();
    shadowsData.mLightView = myGraphicsApi->matrixPolicy(m_shadowCamera.getView());
    shadowsData.mLightProjection = myGraphicsApi->matrixPolicy(m_shadowCamera.getProjection());

    myGraphicsApi->updateConstantBuffer(&lightningData, m_pCB_Lightning);
    myGraphicsApi->updateConstantBuffer(&inverseData, m_pCB_InverseMat);
    myGraphicsApi->updateConstantBuffer(&shadowsData, m_pCB_Shadows);

    //Set textures
    myGraphicsApi->setShaderResourceView(m_pGbuffer_RT->getRenderTexture(0), 0);
    myGraphicsApi->setShaderResourceView(m_pGbuffer_RT->getRenderTexture(1), 1);
    myGraphicsApi->setShaderResourceView(m_pGbuffer_RT->getRenderTexture(3), 2);
    myGraphicsApi->setShaderResourceView(m_pAddition_RT->getRenderTexture(0), 3);
    myGraphicsApi->setShaderResourceView(m_pGbuffer_RT->getRenderTexture(2), 4);
    myGraphicsApi->setShaderResourceView(m_pDepth_RT->getRenderTexture(0), 5);

    m_mySAQ->setSAQ();
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

    //Clear m_rgbaGray
    myGraphicsApi->clearYourRenderTarget(m_pDepth_RT, m_rgbaGray);
    myGraphicsApi->clearYourDepthStencilView(m_pDepth_RT);

    //Update CB
    cbMatrixBuffer matrixData;
    matrixData.mProjection = myGraphicsApi->matrixPolicy(m_shadowCamera.getProjection());
    matrixData.mView = myGraphicsApi->matrixPolicy(m_shadowCamera.getView());
    matrixData.mWorld = m_world.identity();

    //Update CB.
    myGraphicsApi->updateConstantBuffer(&matrixData, m_pCB_Depth);

    //Render model
    mySceneGraph->render();
    m_mySAQ->setSAQ();
  }

  void
  DeferredRendering::shadowMapPass() {
    auto myGraphicsApi = g_graphicApi().instancePtr();
    auto mySceneGraph = SceneGraph::instancePtr();

    //
    myGraphicsApi->setRenderTarget(m_pShadowMap_RT);
    myGraphicsApi->setShaders(m_pShadowMap_Shader);

    //VS CB
    myGraphicsApi->setYourVSConstantBuffers(m_pCB_Shadows, 0);
    myGraphicsApi->setYourVSConstantBuffers(m_pCB_Light, 1);

    //PS CB
    myGraphicsApi->setYourPSConstantBuffers(m_pCB_Light2, 2);

    //Clear
    myGraphicsApi->clearYourRenderTarget(m_pShadowMap_RT, m_rgbaGray2);
    myGraphicsApi->clearYourDepthStencilView(m_pShadowMap_RT);

    //Update CB
    cbLight lightData;
    lightData.lightPosition = m_pLight->getPosition();
    lightData.padding = 0.0f;

    cbLight2 light2Data;
    light2Data.ambientColor = m_pLight->getAmbientColor();
    light2Data.diffuseColor = m_pLight->getDiffuseColor();

    //Update CB.
    myGraphicsApi->updateConstantBuffer(&lightData, m_pCB_Light);
    myGraphicsApi->updateConstantBuffer(&light2Data, m_pCB_Light2);

    //Set textures
    
    //shaderTexture
    myGraphicsApi->setShaderResourceView(m_pGbuffer_RT->getRenderTexture(0), 0);
    
    //depthMapTexture
    myGraphicsApi->setShaderResourceView(m_pDepth_RT->getRenderTexture(0), 1);

    //Render model
    mySceneGraph->render();
  }
}