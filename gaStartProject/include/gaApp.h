#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/cimport.h>

#include <gaCamera.h>
#include <gaModels.h>
#include <gaBaseApp.h>
#include <gaVector4.h>
#include <gaMatrix4x4.h>
#include <gaStructures.h>
#include <gaGraphicsApi.h>

using namespace gaEngineSDK;

class App : public BaseApp
{
  public:
    /*************************************************************************/
    /*
    * Constructor and destructor.
    */
    /*************************************************************************/
    App() = default;
    ~App() = default;

    /*************************************************************************/
    /*
    * Inheritance.
    */
    /*************************************************************************/
    int32
    onInit()override;

    void
    onInitCamera(bool isOGL = false);

    void
    onUpdate(float deltaTime)override;

    void
    onRender()override;

    void
    onCreate()override;

    void
    onDestroySystem()override;

    void
    onKeyboardDown(sf::Event param)override;

    void
    onLeftMouseBtnDown()override;

    void
    onLeftMouseBtnUp()override;

    void
    onMouseMove()override;

  public:
    /*************************************************************************/
    /*
    * Members.
    */
    /*************************************************************************/

    Vector4 m_vMeshColor = (0.7f, 0.7f, 0.7f, 1.0f);


    Matrix4x4 m_world;

    Camera m_mainCamera;
    Model* m_model = nullptr;

    Textures* m_pRenderTargetView = nullptr;
    Textures* m_pDepthStencil = nullptr;

    InputLayout* m_pVertexLayout = nullptr;

    Shaders* m_pBothShaders = nullptr;

    VertexBuffer* m_pVertexBuffer = nullptr;

    IndexBuffer* m_pIndexBuffer = nullptr;

    ConstantBuffer* m_pConstantBuffer1 = nullptr;
    ConstantBuffer* m_pConstantBuffer2 = nullptr;
};