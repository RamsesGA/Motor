#pragma once

#include <gaMesh.h>
#include <gaCamera.h>
#include <gaBaseApp.h>
#include <gaVector4.h>
#include <gaMatrix4x4.h>
#include <gaStructures.h>
#include <gaPrerequisitesCore.h>

namespace gaEngineSDK {
  class ResourceManager;
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
    * @brief Material color.
    */
    Vector4 m_vMeshColor = (0.7f, 0.7f, 0.7f, 1.0f);
  
    /*
    * @brief 4x4 matrix of the world.
    */
    Matrix4x4 m_world;

    /*
    * @brief .
    */
    SPtr<ResourceManager> m_resourceInfo = nullptr;

    /*
    * @brief Variables with the information of the
    *        render target and depth stencil, both of the texture class.
    */
    SPtr<Textures> m_pRenderTargetView = nullptr;
    SPtr<Textures> m_pDepthStencil = nullptr;
  
    /*
    * @brief Variable that stores the vertex layout data.
    */
    SPtr<InputLayout> m_pVertexLayout = nullptr;
  
    /*
    * @brief Variable that saves the data of the shaders to be read.
    */
    SPtr<Shaders> m_pBothShaders = nullptr;
  
    /*
    * @brief Variable that stores the CB data.
    */
    SPtr<ConstantBuffer> m_pBufferCamera = nullptr;
    SPtr<ConstantBuffer> m_pBufferWorld = nullptr;
    SPtr<ConstantBuffer> m_pCBufferBones = nullptr;

    /*
    * @brief .
    */
    SPtr<Mesh> m_mesh;
};