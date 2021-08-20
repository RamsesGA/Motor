#pragma once

#include <gaModule.h>

#include "gaPrerequisitesCore.h"
#include "gaSceneGraph.h"
#include "gaGraphicsApi.h"
#include "gaResourceManager.h"

namespace gaEngineSDK {
  class Shaders;
  class SamplerState;
  class RenderTarget;
  class ConstantBuffer;
  class Plane;

  class GA_CORE_EXPORT BaseRenderer : public Module<BaseRenderer>
  {
   public:
    /*************************************************************************/
    /**
    * Constructor and destructor.
    */
    /*************************************************************************/
    BaseRenderer() = default;

    virtual
    ~BaseRenderer() = default;

    /*************************************************************************/
    /**
    * Methods.
    */
    /*************************************************************************/

    /*
    * @brief .
    */
    void
    setObject(BaseRenderer* api) {
      BaseRenderer::_instance() = api;
    }

    /*
    * @brief .
    */
    virtual void
    init(uint32 width, uint32 height) {};

    /*
    * @brief .
    */
    virtual void
    update(const float& deltaTime) {};

    /*
    * @brief .
    */
    virtual void
    render() {};

    /*
    * @brief .
    */
    virtual void
    resize() {};

    /*
    * @brief Detect keyboard input.
    * @param param, SFML event to know the input pressed.
    * @param deltaTime, time elapsed since the last frame.
    */
    void
    onKeyboardDown(sf::Event param, const float& deltaTime) {
      auto myGraphicsApi = g_graphicApi().instancePtr();

      m_mainCamera.inputDetection(param, deltaTime);

      ConstantBuffer1 cb;
      cb.mView = myGraphicsApi->matrixPolicy(m_mainCamera.getView());

      myGraphicsApi->updateConstantBuffer(&cb, m_pCB_BufferCamera);
    };

    /*
    * @brief Indicates if the left mouse button is pressed.
    */
    void
    onLeftMouseBtnDown() {
      sf::Vector2i position = sf::Mouse::getPosition();

      m_mainCamera.setOriginalMousePos(position.x, position.y);
      m_mainCamera.setClickPressed(true);
    };

    /*
    * @brief Indicates whether the left mouse button was lifted.
    */
    void
    onLeftMouseBtnUp() {
      m_mainCamera.setClickPressed(false);
    };

    /*
    * @brief Detects if the mouse is moving on the screen.
    */
    void
    onMouseMove(const float& deltaTime) {
      if (m_mainCamera.getClickPressed()) {
        auto myGraphicsApi = g_graphicApi().instancePtr();

        m_mainCamera.setOriginalMousePos(m_mainCamera.getOriginalMousePos().x,
          m_mainCamera.getOriginalMousePos().y);

        m_mainCamera.mouseRotation(deltaTime);

        ConstantBuffer1 cb;
        cb.mView = myGraphicsApi->matrixPolicy(m_mainCamera.getView());

        myGraphicsApi->updateConstantBuffer(&cb, m_pCB_BufferCamera);
      }
    };

    void
    defaultCamera() {
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

    /*************************************************************************/
    /*
    * Passes.
    */
    /*************************************************************************/

    /*
    * @brief .
    */
    virtual void
    RTGbufferPass() {};

    /*
    * @brief .
    */
    virtual void
    RT_SSAO_Pass() {};

    /*
    * @brief .
    */
    virtual void
    BlurH_Pass(void* texture) {};

    /*
    * @brief .
    */
    virtual void
    BlurV_Pass(void* texture) {};

    /*
    * @brief .
    */
    virtual void
    AdditionPass(void* texture1, void* texture2) {};

    /*
    * @brief .
    */
    virtual void
    LightningPass() {};

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
    Vector4 m_rgba = { (0.0f / 255.0f), (35.0f / 255.0f), (130.0f / 255.0f), (255.0f) };

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
    SPtr<Shaders> m_pSSAO_Shader;
    SPtr<Shaders> m_pBlurH_Shader;
    SPtr<Shaders> m_pBlurV_Shader;
    SPtr<Shaders> m_pLightning_Shader;
    SPtr<Shaders> m_pAddition_Shader;

    /*
    * @brief Members for renter targets.
    */
    SPtr<RenderTarget> m_pGbuffer_RT;
    SPtr<RenderTarget> m_pSSAO_RT;
    SPtr<RenderTarget> m_pBlurH_RT;
    SPtr<RenderTarget> m_pBlurV_RT;
    SPtr<RenderTarget> m_pAddition_RT;
    SPtr<RenderTarget> m_pLightning_RT;

    /*
    * @brief Variable that stores the CB data.
    */
    SPtr<ConstantBuffer> m_pCB_BufferCamera;
    SPtr<ConstantBuffer> m_pCB_BufferWorld;
    SPtr<ConstantBuffer> m_pCB_BufferBones;
    SPtr<ConstantBuffer> m_pCB_SSAO;
    SPtr<ConstantBuffer> m_pCB_ViewPortDimension;
    SPtr<ConstantBuffer> m_pCB_Lightning;
    SPtr<ConstantBuffer> m_pCB_MipLevels;

    /*
    * @brief .
    */
    SPtr<SamplerState> m_pSampler;

    /*
    * @brief Variable that stores the vertex layout data.
    */
    SPtr<InputLayout> m_pVertexLayout;

    /*
    * @brief .
    */
    SPtr<Mesh> m_mesh;

    /*
    * @brief .
    */
    SPtr<Plane> m_mySAQ;

    friend class DeferredRendering;
  };

  /***************************************************************************/
  /**
  * INSTANCE.
  */
  /***************************************************************************/

  GA_CORE_EXPORT BaseRenderer&
  g_baseRenderer();
}