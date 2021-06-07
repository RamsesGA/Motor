#pragma once

#include <exception>

#include <gaCamera.h>
#include <gaBaseApp.h>
#include <gaVector4.h>
#include <gaMatrix4x4.h>
#include <gaStructures.h>
#include <gaGraphicsApi.h>
#include <gaMesh.h>

namespace gaEngineSDK {
  class Models;
  class ResourceManager;
}

using namespace gaEngineSDK;
using std::exception;

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

    int32
    onInit()override;

    void
    onInitCamera();

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
  
    /*
    * @brief Material color.
    */
    Vector4 m_vMeshColor = (0.7f, 0.7f, 0.7f, 1.0f);
  
    /*
    * @brief 4x4 matrix of the world.
    */
    Matrix4x4 m_world;
  
    /*
    * @brief Variable with camera information.
    */
    Camera m_mainCamera;
  
    /*
    * @brief Variable with the information of the loaded model.
    */
    Model* m_model = nullptr;

    /*
    * @brief .
    */
    ResourceManager* m_resourceManager = nullptr;
  
    /*
    * @brief Variables with the information of the
    *        render target and depth stencil, both of the texture class.
    */
    Textures* m_pRenderTargetView = nullptr;
    Textures* m_pDepthStencil = nullptr;
  
    /*
    * @brief Variable that stores the vertex layout data.
    */
    InputLayout* m_pVertexLayout = nullptr;
  
    /*
    * @brief Variable that saves the data of the shaders to be read.
    */
    Shaders* m_pBothShaders = nullptr;
  
    /*
    * @brief Variable that stores the CB data.
    */
    ConstantBuffer* m_pConstantBuffer1 = nullptr;
    ConstantBuffer* m_pConstantBuffer2 = nullptr;

    /*
    * @brief .
    */
    Mesh* m_mesh;
};