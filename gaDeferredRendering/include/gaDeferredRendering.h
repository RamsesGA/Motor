#pragma once

#include <gaBaseRenderer.h>
#include <gaLights.h>
#include <gaBaseInputs.h>

#include "gaPrerequisitesDefRend.h"

namespace gaEngineSDK {
  class DeferredRendering final : public BaseRenderer
  {
   public:
    /*************************************************************************/
    /**
    * Constructor and destructor.
    */
    /*************************************************************************/

    DeferredRendering() = default;

    ~DeferredRendering() = default;

    /*************************************************************************/
    /**
    * Inheritance.
    */
    /*************************************************************************/

    /*
    * @brief .
    */
    void
    init(uint32 width, uint32 height) override;

    /*
    * @brief .
    */
    void
    update(const float& deltaTime) override;

    /*
    * @brief .
    */
    void
    render() override;

    /*
    * @brief .
    */
    void
    resize() override;

    /*
    * @brief .
    */
    void
    keyboardButtons(const float& deltaTime);

    /*
    * @brief .
    */
    void
    mouseRotation(const float& deltaTime);

    /*
    * @brief .
    */
    void
    onLeftMouseBtnDown() override;

    /*
    * @brief .
    */
    void
    defaultCamera() override;

    /*************************************************************************/
    /*
    * Info passes.
    */
    /*************************************************************************/

    void
    setGBuffer();

    void
    createBlurSSAO();

    void
    createCSBlurSSAO();

    void
    createBlurDepth();

    /*************************************************************************/
    /*
    * Passes.
    */
    /*************************************************************************/

    /*
    * @brief .
    */
    void
    gbufferPass();

    /*
    * @brief .
    */
    void
    SSAO_Pass();

    /*
    * @brief .
    */
    void
    CS_SSAO();

    /*
    * @brief .
    */
    void
    blurH_Pass(void* texture);

    void
    csBlurHPass(void* texture);

    /*
    * @brief .
    */
    void
    blurV_Pass(void* texture);

    void
    csBlurVPass(void* texture);

    /*
    * @brief .
    */
    void
    additionPass(void* texture1, void* texture2);

    /*
    * @brief .
    */
    void
    additionDepthPass(void* texture1, void* texture2);

    /*
    * @brief .
    */
    void
    lightningPass();

    /*
    * @brief .
    */
    void
    depthPass();

    /*************************************************************************/
    /*
    * .
    */
    /*************************************************************************/

    /*
    * @brief .
    */
    void
    defaultLight();

    /*
    * @brief .
    */
    void
    createSamplers();

    /*
    * @brief .
    */
    void
    createRenderTargets();

    /*
    * @brief .
    */
    void
    createBuffers();

    /*
    * @brief .
    */
    void
    loadShadersFiles();

   protected:
    /*************************************************************************/
    /**
    * Members.
    */
    /*************************************************************************/

    /*
    * @param Members with screen size.
    */
    uint32 m_width = 0;
    uint32 m_height = 0;

    /*
    * @brief Variable with camera information.
    */
    Camera m_mainCamera;

    /*
    * @brief 4x4 matrix of the world.
    */
    Matrix4x4 m_world;

    /*
    * @brief .
    */
    Vector4 m_rgbaMagenta = { (207.0f / 255.0f), (52.0f / 255.0f), (118.0f / 255.0f), 255.0f };

    /*
    * @brief .
    */
    Vector4 m_rgbaCyan = { 0.0f, (255.0f / 255.0f), (255.0f / 255.0f), 255.0f };

    /*
    * @brief .
    */
    Vector4 m_rgbaOrange = { (255.0f / 255.0f), (165.0f / 255.0f), 0.0f, 255.0f };

    /*
    * @brief .
    */
    Vector4 m_rgbaGreen = { 0.0f, (255.0f / 255.0f), 0.0f, 255.0f };

    /*
    * @brief .
    */
    Vector4 m_rgbaBlue = { 0.0f, 0.0f, (255.0f / 255.0f), 255.0f };

    /*
    * @brief .
    */
    Vector4 m_rgbaYellow = { (255.0f / 255.0f), (255.0f / 255.0f), 0.0f, 255.0f };

    /*
    * @brief Variables with the information of the
    *    render target and depth stencil, both of the texture class.
    */
    SPtr<Textures> m_pRenderTargetView;
    SPtr<Textures> m_pDepthStencil;

    /*
    * @brief Variable that saves the data of the shaders to be read.
    */
    SPtr<Shaders> m_pGBuffer_Shader;
    //SPtr<Shaders> m_pSSAO_Shader;
    //SPtr<Shaders> m_pBlurH_Shader;
    //SPtr<Shaders> m_pBlurV_Shader;
    SPtr<Shaders> m_pLightning_Shader;
    //SPtr<Shaders> m_pAddition_Shader;
    //SPtr<Shaders> m_pAdditionDepth_Shader;
    SPtr<Shaders> m_pDepth_Shader;

    //Compute shaders
    SPtr<Shaders> m_pCS_SSAO;
    SPtr<Shaders> m_pCS_BlurH;
    SPtr<Shaders> m_pCS_BlurV;

    /*
    * @brief Members for renter targets.
    */
    SPtr<RenderTarget> m_pGbuffer_RT;
    //SPtr<RenderTarget> m_pSSAO_RT;
    //SPtr<RenderTarget> m_pBlurH_RT;
    //SPtr<RenderTarget> m_pBlurV_RT;
    //SPtr<RenderTarget> m_pAddition_RT;
    //SPtr<RenderTarget> m_pAdditionShadow_RT;
    SPtr<RenderTarget> m_pLightning_RT;

    //Depth map
    SPtr<RenderTarget> m_pDepth_RT;

    //Compute
    SPtr<RenderTarget> m_pCompSSAO_RT;
    SPtr<RenderTarget> m_pCompBlurH_RT;
    SPtr<RenderTarget> m_pCompBlurV_RT;

    /*
    * @brief Variable that stores the CB data.
    */
    SPtr<ConstantBuffer> m_pCB_BufferCamera;
    SPtr<ConstantBuffer> m_pCB_BufferWorld;
    SPtr<ConstantBuffer> m_pCB_BufferBones;
    SPtr<ConstantBuffer> m_pCB_SSAO;
    SPtr<ConstantBuffer> m_pCB_SSAO2;
    SPtr<ConstantBuffer> m_pCB_ViewPortDimension;
    SPtr<ConstantBuffer> m_pCB_Lightning;
    SPtr<ConstantBuffer> m_pCB_MipLevels;

    //Depth map
    SPtr<ConstantBuffer> m_pCB_Depth;
    SPtr<ConstantBuffer> m_pCB_InverseMat;
    SPtr<ConstantBuffer> m_pCB_Shadows;

    /*
    * @brief Samplers.
    */
    SPtr<SamplerState> m_pSampler;
    SPtr<SamplerState> m_pCSSampler;
    SPtr<SamplerState> m_pSampleStateClamp;

    /*
    * @brief Variable that stores the vertex layout data.
    */
    SPtr<InputLayout> m_pVertexLayout;
    SPtr<InputLayout> m_pDepthLayout;

    /*
    * @brief .
    */
    SPtr<Mesh> m_mesh;

    /*
    * @brief .
    */
    SPtr<Plane> m_mySAQ;

    /*
    * @brief .
    */
    SPtr<Actor> m_pLightActor;
  };

  /***************************************************************************/
  /**
  * Export.
  */
  /***************************************************************************/
  extern "C" GA_DEF_REND_EXPORT BaseRenderer *
  createBaseRenderer() {
    DeferredRendering* pDefRender = new DeferredRendering();
    return pDefRender;
  }
}