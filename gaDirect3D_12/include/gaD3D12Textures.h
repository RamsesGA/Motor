#pragma once

#include <d3d12.h>

#include <gaTextures.h>

namespace gaEngineSDK {
  class D3D12Textures final : public Textures
  {
  public:
    /*************************************************************************/
    /**
    * Constructor and destructor.
    */
    /*************************************************************************/
    D3D12Textures() = default;

    ~D3D12Textures() = default;

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
      //if ((index >= m_vShaderResourceView.size()) || (0 > index)) {
      //  return m_vShaderResourceView[0];
      //}
      //return m_vShaderResourceView[index];
    };

  protected:
    /*************************************************************************/
    /**
    * Members.
    */
    /*************************************************************************/

    /**
    * @brief DX member to save the texture.
    * ID3D11Texture2D* m_pTexture = nullptr;
    */

    /**
    * @brief DX vector member to save the resource view shader.
    * Vector<ID3D11ShaderResourceView*> m_vShaderResourceView;
    */
    

    /**
    * @brief DX member to save the depth stencil view.
    * ID3D11DepthStencilView* m_pDepthStencilView = nullptr;
    */
    

    /**
    * @brief DX member to save the render target view.
    * Vector<ID3D11RenderTargetView*> m_vRenderTargetView;
    */
    

    /**
    * @brief DX member to save the depth stencil state.
    * ID3D11DepthStencilState* m_pDepthStencilState = nullptr;
    */
    

    /**
    * @brief .
    * ID3D11UnorderedAccessView* m_pUAV = nullptr;
    */


    

    friend class D3D12GraphicsApi;
    friend class D3D12RenderTarget;
  };
}