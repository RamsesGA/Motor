#pragma once

#include <gaMesh.h>
#include <gaCamera.h>
#include <gaBaseApp.h>
#include <gaVector4.h>
#include <gaMatrix4x4.h>
#include <gaStructures.h>
#include <gaSamplerState.h>
#include <gaRenderTarget.h>
#include <gaPrerequisitesCore.h>

namespace gaEngineSDK {
  class ResourceManager;
  class StaticMesh;
}

class AppTest : public BaseApp 
{
  public:
    /*************************************************************************/
    /*
    * Constructor and destructor.
    */
    /*************************************************************************/
    AppTest() = default;
  
    ~AppTest() = default;
  
    /*************************************************************************/
    /*
    * Inheritance.
    */
    /*************************************************************************/

    void
    onInitCamera();

    /*
    * @brief .
    */
    void
    onUpdate(float deltaTime) override;

    /*
    * @brief .
    */
    void
    onRender() override;

    /*
    * @brief .
    */
    void
    onCreate() override;

    /*
    * @brief .
    */
    void
    onDestroySystem() override;

    /*
    * @brief .
    */
    void
    onKeyboardDown(sf::Event param, const float& deltaTime) override;

    /*
    * @brief .
    */
    void
    onLeftMouseBtnDown() override;

    /*
    * @brief .
    */
    void
    onLeftMouseBtnUp() override;

    /*
    * @brief .
    */
    void
    onMouseMove() override;

    /*************************************************************************/
    /*
    * Methods.
    */
    /*************************************************************************/

    /*
    * @brief .
    */
    void
    setRT_View();

    /*
    * @brief .
    */
    void
    setRTGbuffer();

    /*
    * @brief .
    */
    void
    setRT_SSAO();

    /*
    * @brief .
    */
    void
    setBlurH();

    /*
    * @brief .
    */
    void
    setBlurV();

    /*
    * @brief .
    */
    void
    createNodePod();

    /*
    * @brief .
    */
    void
    createNodeVela();

    /*
    * @brief .
    */
    void
    createNodeTwoB();

    /*
    * @brief .
    */
    void
    createNodeSpartan();

    /*
    * @brief .
    */
    void
    createNodeUgandan();

    /*
    * @brief .
    */
    void
    createNodeGrimoires();

    /*
    * @brief .
    */
    void
    createNodeRamlethalSwords();

    /*
    * @brief .
    */
    void
    createSAQ();

    /*
    * @brief .
    */
    void
    setSAQ();

  public:
    /*************************************************************************/
    /*
    * Members.
    */
    /*************************************************************************/

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
    Vector4 m_rgba = { (87.0f / 255.0f), (35.0f / 255.0f), (100.0f / 255.0f), (255.0f) };

    /*
    * @brief .
    */
    Vector<SPtr<StaticMesh>> m_vStaticMeshes;

    /*
    * @brief .
    */
    SPtr<Models> m_SAQ;

    /*
    * @brief .
    */
    SPtr<ResourceManager> m_resourceInfo;

    /*
    * @brief Variables with the information of the
    *        render target and depth stencil, both of the texture class.
    */
    SPtr<Textures> m_pRenderTargetView;
    SPtr<Textures> m_pDepthStencil;
  
    /*
    * @brief Variable that stores the vertex layout data.
    */
    SPtr<InputLayout> m_pVertexLayout = nullptr;
  
    /*
    * @brief Variable that saves the data of the shaders to be read.
    */
    SPtr<Shaders> m_pGBufferShader;
    SPtr<Shaders> m_pSSAO_Shader;
    SPtr<Shaders> m_pBlurH_Shader;
    SPtr<Shaders> m_pBlurV_Shader;
    SPtr<Shaders> m_pLightningShader;

    /*
    * @brief Members for renter targets.
    */
    SPtr<RenderTarget> m_pGbufferRT;
    SPtr<RenderTarget> m_pSSAO_RT;
    SPtr<RenderTarget> m_pBlurH_RT;
    SPtr<RenderTarget> m_pBlurV_RT;
  
    /*
    * @brief Variable that stores the CB data.
    */
    SPtr<ConstantBuffer> m_pBufferCamera;
    SPtr<ConstantBuffer> m_pBufferWorld;
    SPtr<ConstantBuffer> m_pCBufferBones;

    SPtr<ConstantBuffer> m_pCB_SSAO;
    SPtr<ConstantBuffer> m_pCB_ViewPortDimension;
    SPtr<ConstantBuffer> m_pCB_Lightning;

    /*
    * @brief .
    */
    SPtr<Mesh> m_mesh;

    /*
    * @brief .
    */
    SPtr<SamplerState> m_pSampler;
};