#pragma once

#include <gaBaseApp.h>
#include <gaCamera.h>
#include <gaModels.h>
#include <gaVector4.h>
#include <gaMatrix4x4.h>
#include <gaGraphicsApi.h>

using namespace gaEngineSDK;

/***************************************************************************/
/**
* Structs
*/
/***************************************************************************/
namespace ConstantBuffer1 {
  struct E {
    Matrix4x4 mView;
    Matrix4x4 mProjection;
  };
}

namespace ConstantBuffer2 {
  struct E {
    Matrix4x4 mWorld;
    Vector4 vMeshColor;
  };
}

namespace Matrices {
  struct E {
    Matrix4x4 World;
    Matrix4x4 Projection;
    Vector4   Color;
  };
}

namespace ViewCB {
  struct E {
    Matrix4x4 View;
  };
}

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

    GraphicsApi* m_pGraphicApi;

    Textures* m_pRenderTargetView = nullptr;
    Textures* m_pDepthStencil = nullptr;

    InputLayout* m_pVertexLayout = nullptr;

    Shaders* m_pBothShaders = nullptr;

    VertexBuffer* m_pVertexBuffer = nullptr;

    IndexBuffer* m_pIndexBuffer = nullptr;

    ConstantBuffer* m_pConstantBuffer1 = nullptr;
    ConstantBuffer* m_pConstantBuffer2 = nullptr;
};