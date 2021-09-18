#include <gaGraphicsApi.h>
#include <gaPlane.h>
#include <gaRenderTarget.h>

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

    m_pDepth_Shader.reset(myGraphicsApi->createShadersProgram(L"data/shaders/DX_Depth.hlsl",
                                                              "DepthVS",
                                                              L"data/shaders/DX_Depth.hlsl",
                                                              "DepthPS"));

    m_pShadowMap_Shader.reset(myGraphicsApi->createShadersProgram(
                                             L"data/shaders/DX_ShadowMap.hlsl",
                                             "ShadowVS",
                                             L"data/shaders/DX_ShadowMap.hlsl",
                                             "ShadowPS"));

    //We create the input layout.
    m_pVertexLayout.reset(myGraphicsApi->createInputLayout(m_pGBuffer_Shader));

    //We create the input layout.
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
    m_pCB_BufferCamera.reset(myGraphicsApi->createConstantBuffer(sizeof(ConstantBuffer1)));
    m_pCB_BufferWorld.reset(myGraphicsApi->createConstantBuffer(sizeof(ConstantBuffer2)));
    m_pCB_BufferBones.reset(myGraphicsApi->createConstantBuffer(sizeof(ConstBuffBonesTransform)));
    m_pCB_SSAO.reset(myGraphicsApi->createConstantBuffer(sizeof(cbSSAO)));
    m_pCB_ViewPortDimension.reset(myGraphicsApi->createConstantBuffer(sizeof(cbViewportDimension)));
    m_pCB_Lightning.reset(myGraphicsApi->createConstantBuffer(sizeof(cbLightning)));
    m_pCB_MipLevels.reset(myGraphicsApi->createConstantBuffer(sizeof(cbMipLevels)));

    //Depth
    m_pCB_Matrix.reset(myGraphicsApi->createConstantBuffer(sizeof(cbMatrixBuffer), 
                                                           CPU_ACCESS::kCpuAccessWrite,
                                                           USAGE::kUsageDynamic));
    //Shadow map
    m_pCB_Shadows.reset(myGraphicsApi->createConstantBuffer(sizeof(cbShadows), 
                                                            CPU_ACCESS::kCpuAccessWrite,
                                                            USAGE::kUsageDynamic));

    m_pCB_Light.reset(myGraphicsApi->createConstantBuffer(sizeof(cbLight), 
                                                          CPU_ACCESS::kCpuAccessWrite,
                                                          USAGE::kUsageDynamic));

    m_pCB_Light2.reset(myGraphicsApi->createConstantBuffer(sizeof(cbLight2), 
                                                           CPU_ACCESS::kCpuAccessWrite,
                                                           USAGE::kUsageDynamic));
    /*
    * C R E A T E
    * R E N D E R
    * T A R G E TS
    * Z O N E
    */
    m_pGbuffer_RT = make_shared<RenderTarget>();
    m_pSSAO_RT = make_shared<RenderTarget>();
    m_pBlurH_RT = make_shared<RenderTarget>();
    m_pBlurV_RT = make_shared<RenderTarget>();
    m_pAddition_RT = make_shared<RenderTarget>();
    m_pLightning_RT = make_shared<RenderTarget>();

    //Depth
    m_pMatrix_RT = make_shared<RenderTarget>();

    //Shadow map
    m_pShadowMap_RT = make_shared<RenderTarget>();

    m_pGbuffer_RT = myGraphicsApi->createRenderTarget(m_width, m_height, 1, 4);
    m_pSSAO_RT = myGraphicsApi->createRenderTarget(m_width, m_height);
    m_pBlurH_RT = myGraphicsApi->createRenderTarget(m_width, m_height);
    m_pBlurV_RT = myGraphicsApi->createRenderTarget(m_width, m_height);
    m_pAddition_RT = myGraphicsApi->createRenderTarget(m_width, m_height);
    m_pLightning_RT = myGraphicsApi->createRenderTarget(m_width, m_height);

    //Depth
    m_pMatrix_RT = myGraphicsApi->createRenderTarget(m_width, m_height);

    //Shadow map
    m_pShadowMap_RT = myGraphicsApi->createRenderTarget(m_width, m_height);

    /*
    * C R E A T E
    * T E X T U R E S
    * Z O N E
    */
    m_depthTexture = myGraphicsApi->createTexture(m_width, 
                                                  m_height, 
                                                  D3D11_BIND_SHADER_RESOURCE,
                                                  TEXTURE_FORMAT::E::kR16Float);

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

    m_pSampleStateWrap = myGraphicsApi->createSamplerState(
                                        FILTER::kFilterMinMagMipLinear,
                                        TEXTURE_ADDRESS::kTextureAddressWrap,
                                        COMPARISON::kComparisonAlways);

    myGraphicsApi->setSamplerState(m_pSampler, 0);
    myGraphicsApi->setSamplerState(m_pSampleStateClamp, 1);
    myGraphicsApi->setSamplerState(m_pSampleStateWrap, 2);

    //Create the one face of cube.
    m_mySAQ.reset(new Plane());
    m_mySAQ->createSAQ();
  }

  void
  DeferredRendering::update(const float& deltaTime) {
    auto myGraphicsApi = g_graphicApi().instancePtr();

    /*
    * P A S S E S
    * Z O N E
    */
    rtGbufferPass();
    depthPass();
    //shadowMapPass();
    rtSSAO_Pass();
    blurH_Pass(m_pSSAO_RT->getRenderTexture(0));
    blurV_Pass(m_pSSAO_RT->getRenderTexture(0));
    additionPass(m_pBlurH_RT->getRenderTexture(0), m_pBlurV_RT->getRenderTexture(0));

    for (uint32 i = 0; i < 2; ++i) {
      blurH_Pass(m_pAddition_RT->getRenderTexture(0));
      blurV_Pass(m_pAddition_RT->getRenderTexture(0));
      additionPass(m_pBlurH_RT->getRenderTexture(0), m_pBlurV_RT->getRenderTexture(0));
    }

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

  void
  DeferredRendering::onKeyboardDown(Event param, const float& deltaTime) {
    auto myGraphicsApi = g_graphicApi().instancePtr();

    m_mainCamera.inputDetection(param, deltaTime);

    ConstantBuffer1 cb;
    cb.mView = myGraphicsApi->matrixPolicy(m_mainCamera.getView());

    myGraphicsApi->updateConstantBuffer(&cb, m_pCB_BufferCamera);
  }

  void
  DeferredRendering::onLeftMouseBtnDown() {
    Vector2i position = Mouse::getPosition();

    m_mainCamera.setOriginalMousePos(position.x, position.y);
    m_mainCamera.setClickPressed(true);
  }

  void
  DeferredRendering::onLeftMouseBtnUp() {
    m_mainCamera.setClickPressed(false);
  }

  void
  DeferredRendering::onMouseMove(const float& deltaTime) {
    if (m_mainCamera.getClickPressed()) {
      auto myGraphicsApi = g_graphicApi().instancePtr();

      m_mainCamera.setOriginalMousePos(m_mainCamera.getOriginalMousePos().x,
                                       m_mainCamera.getOriginalMousePos().y);

      m_mainCamera.mouseRotation(deltaTime);

      ConstantBuffer1 cb;
      cb.mView = myGraphicsApi->matrixPolicy(m_mainCamera.getView());

      myGraphicsApi->updateConstantBuffer(&cb, m_pCB_BufferCamera);
    }
  }

  void
  DeferredRendering::defaultCamera() {
    m_mainCamera.setLookAt();
    m_mainCamera.setEye();
    m_mainCamera.setUp();
    m_mainCamera.setFar();
    m_mainCamera.setNear();
    m_mainCamera.setFoV();
    m_mainCamera.setWidth(m_width);
    m_mainCamera.setHeight(m_height);
    m_mainCamera.startCamera();
  }

  void
  DeferredRendering::rtGbufferPass() {
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
    myGraphicsApi->setConstBufferBones(m_pCB_BufferBones);

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

    myGraphicsApi->clearYourRenderTarget(m_pGbuffer_RT, m_rgbaBlue);

    ConstantBuffer1 meshData;
    meshData.mProjection = myGraphicsApi->matrixPolicy(m_mainCamera.getProjection());
    meshData.mView = myGraphicsApi->matrixPolicy(m_mainCamera.getView());

    ConstantBuffer2 cb;
    cb.mWorld = m_world;
    cb.objectPosition = { 0.0f, 0.0f, 0.0f };

    //Update CB.
    myGraphicsApi->updateConstantBuffer(&meshData, m_pCB_BufferCamera);
    myGraphicsApi->updateConstantBuffer(&cb, m_pCB_BufferWorld);

    //Render model
    mySceneGraph->render();
  }

  void 
  DeferredRendering::rtSSAO_Pass() {
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

    //Clean RT
    myGraphicsApi->clearYourRenderTarget(m_pAddition_RT, m_rgbaBlue);

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

    //Clean RT
    myGraphicsApi->clearYourRenderTargetView(m_pRenderTargetView, m_rgbaBlue);

    //Update CB
    cbLightning lightningData;
    lightningData.emissiveIntensity = 1.0f;
    lightningData.lightIntensity0 = 2.0f;
    //TODO: check lightPos, the information in shader
    //lightningData.lightPos0 = { 0.0f, 0.0f, 0.0f };
    lightningData.vViewPosition = m_mainCamera.getCamEye();

    myGraphicsApi->updateConstantBuffer(&lightningData, m_pCB_Lightning);

    //Set textures
    myGraphicsApi->setShaderResourceView(m_pGbuffer_RT->getRenderTexture(0), 0);
    myGraphicsApi->setShaderResourceView(m_pGbuffer_RT->getRenderTexture(1), 1);
    myGraphicsApi->setShaderResourceView(m_pGbuffer_RT->getRenderTexture(3), 2);
    myGraphicsApi->setShaderResourceView(m_pAddition_RT->getRenderTexture(0), 3);
    myGraphicsApi->setShaderResourceView(m_pGbuffer_RT->getRenderTexture(2), 4);

    m_mySAQ->setSAQ();
  }

  void 
  DeferredRendering::depthPass() {
    auto myGraphicsApi = g_graphicApi().instancePtr();
    auto mySceneGraph = SceneGraph::instancePtr();

    //Defining shadow camera's value
    m_shadowCamera.setLookAt(Vector3(-100.0f, 250.0f, -100.0f));
    m_shadowCamera.setEye();
    m_shadowCamera.setUp();
    m_shadowCamera.setFar();
    m_shadowCamera.setNear();
    m_shadowCamera.setFoV();
    m_shadowCamera.setWidth(m_width);
    m_shadowCamera.setHeight(m_height);
    m_shadowCamera.startCamera();

    //We save the input layout.
    myGraphicsApi->setInputLayout(m_pDepthLayout);

    myGraphicsApi->setRenderTarget(m_pMatrix_RT);
    myGraphicsApi->setShaders(m_pDepth_Shader);

    //VS CB
    myGraphicsApi->setYourVSConstantBuffers(m_pCB_Matrix, 0);

    //Clear
    myGraphicsApi->clearYourRenderTarget(m_pMatrix_RT, m_rgbaOrange);

    //Update CB
    cbMatrixBuffer matrixData;
    matrixData.mProjection = myGraphicsApi->matrixPolicy(m_shadowCamera.getProjection());
    matrixData.mView = myGraphicsApi->matrixPolicy(m_shadowCamera.getView());
    matrixData.mWorld = m_world.identity();

    //Update CB.
    myGraphicsApi->updateConstantBuffer(&matrixData, m_pCB_Matrix);

    //Render model
    mySceneGraph->render();
  }

  void
  DeferredRendering::shadowMapPass() {
    auto myGraphicsApi = g_graphicApi().instancePtr();
    auto mySceneGraph = SceneGraph::instancePtr();

    //Light info
    m_pLight->generateViewMatrix();
    m_pLight->setAmbientColor(0.15f, 0.15f, 0.15f, 1.0f);
    m_pLight->setDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
    m_pLight->setLookAt(-100.0f, 250.0f, -100.0f);
    m_pLight->setPosition(0.0f, 0.0f, 0.0f);

    Matrix4x4 lightVM;
    Matrix4x4 lightPM;
    m_pLight->getViewMatrix(lightVM);
    m_pLight->getProjectionMatrix(lightPM);

    //
    myGraphicsApi->setRenderTarget(m_pShadowMap_RT);
    myGraphicsApi->setShaders(m_pShadowMap_Shader);

    //VS CB
    myGraphicsApi->setYourVSConstantBuffers(m_pCB_Shadows, 0);
    myGraphicsApi->setYourVSConstantBuffers(m_pCB_Light, 1);

    //PS CB
    myGraphicsApi->setYourPSConstantBuffers(m_pCB_Light2, 2);

    //Clear
    myGraphicsApi->clearYourRenderTarget(m_pShadowMap_RT, Vector4(0.0f, 1.0f, 0.0f, 1.0f));

    //Update CB
    cbShadows shadowsData;
    shadowsData.mLightProjection = lightPM;
    shadowsData.mLightView = lightVM;
    shadowsData.mProjection = myGraphicsApi->matrixPolicy(m_shadowCamera.getProjection());
    shadowsData.mView = myGraphicsApi->matrixPolicy(m_shadowCamera.getView());
    shadowsData.mWorld = m_world;

    cbLight lightData;
    lightData.lightPosition = m_pLight->getPosition();
    lightData.padding = 0.0f;

    cbLight2 light2Data;
    light2Data.ambientColor = m_pLight->getAmbientColor();
    light2Data.diffuseColor = m_pLight->getDiffuseColor();

    //Update CB.
    myGraphicsApi->updateConstantBuffer(&shadowsData, m_pCB_Shadows);
    myGraphicsApi->updateConstantBuffer(&lightData, m_pCB_Light);
    myGraphicsApi->updateConstantBuffer(&light2Data, m_pCB_Light2);

    //Set textures
    myGraphicsApi->setShaderResourceView(nullptr, 0);//shaderTexture  m_pGbuffer_RT->getRenderTexture(1)
    myGraphicsApi->setShaderResourceView(nullptr, 1);//depthMapTexture

    //Render model
    mySceneGraph->render();
  }
}