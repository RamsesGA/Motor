#pragma once

#include "gaRenderTarget.h"
#include "gaTexturesDX.h"

namespace gaEngineSDK {
  class RenderTargetDX final : public RenderTarget
  {
  public:
    /*************************************************************************/
    /**
    * Constructor and destructor.
    */
    /*************************************************************************/
    RenderTargetDX() = default;

    ~RenderTargetDX() = default;

    /*************************************************************************/
    /**
    * Methods.
    */
    /*************************************************************************/

    /*
    * @brief .
    */
    void*
    getRenderTexture(uint32 index = 0) override { 
      if ((index >= m_renderTarget.m_vShaderResourceView.size()) || (0 > index)) {
        return m_renderTarget.m_vShaderResourceView[0];
      }

      return m_renderTarget.m_vShaderResourceView[index];
    };

  protected:
    /*************************************************************************/
    /**
    * Members.
    */
    /*************************************************************************/

    /*
    * @brief .
    */
    TexturesDX m_renderTarget;

    /*
    * @brief .
    */
    ID3D11DepthStencilView* m_pDepthStencil = nullptr;

    friend class GraphicsApiDX;
  };
}