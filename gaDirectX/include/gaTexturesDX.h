#pragma once

#include <d3d11.h>
#include <gaTextures.h>
#include <gaComputeBufferDX.h>

namespace gaEngineSDK {
  class TexturesDX final : public Textures
  {
  public:
    /*************************************************************************/
    /**
    * Constructor and destructor.
    */
    /*************************************************************************/
    TexturesDX() = default;

    ~TexturesDX() {
      SAFE_RELEASE(m_pTexture);
      SAFE_RELEASE(m_pDepthStencilView);
      SAFE_RELEASE(m_pDepthStencilState);
      SAFE_RELEASE(m_pUAV);

      if (0 != m_vShaderResourceView.size()) {
        uint32 tempSize = m_vShaderResourceView.size();
        for (uint32 i = 0; i < tempSize; ++i) {
          SAFE_RELEASE(m_vShaderResourceView[i]);
        }
      }
      if (0 != m_vRenderTargetView.size()) {
        uint32 tempSize = m_vRenderTargetView.size();
        for (uint32 i = 0; i < tempSize; ++i) {
          SAFE_RELEASE(m_vRenderTargetView[i]);
        }
      }
    };

    /*************************************************************************/
    /**
    * Hereditary methods.
    */
    /*************************************************************************/

    /*
    * @brief Return the shader resource view.
    */
    void*
    getTexture(uint32 index = 0) override {
      if ((index >= m_vShaderResourceView.size()) || (0 > index)) {
        return m_vShaderResourceView[0];
      }
      return m_vShaderResourceView[index];
    };

  protected:
    /*************************************************************************/
    /**
    * Members.
    */
    /*************************************************************************/

    /**
    * @brief DX member to save the texture.
    */
    ID3D11Texture2D* m_pTexture = nullptr;

    /**
    * @brief DX vector member to save the resource view shader.
    */
    Vector<ID3D11ShaderResourceView*> m_vShaderResourceView;

    /**
    * @brief DX member to save the depth stencil view.
    */
    ID3D11DepthStencilView* m_pDepthStencilView = nullptr;

    /**
    * @brief DX member to save the render target view.
    */
    Vector<ID3D11RenderTargetView*> m_vRenderTargetView;

    /**
    * @brief DX member to save the depth stencil state.
    */
    ID3D11DepthStencilState* m_pDepthStencilState = nullptr;

    /**
    * @brief .
    */
    ID3D11UnorderedAccessView* m_pUAV = nullptr;

    /**
    * @brief .
    */
    SPtr<ComputeBufferDX> m_pCompBuff;

    friend class GraphicsApiDX;
    friend class RenderTargetDX;
  };
}