#pragma once

#include <d3d12.h>

#include <gaPixelShader.h>

namespace gaEngineSDK {
  class D3D12PixelShader final : public PixelShader
  {
  public:
    /*************************************************************************/
    /**
    * Constructor and destructor.
    */
    /*************************************************************************/
    D3D12PixelShader() = default;

    ~D3D12PixelShader() = default;

  protected:
    /*************************************************************************/
    /**
    * Members.
    */
    /*************************************************************************/

    /**
    * @brief pointer to direct 11 pixel shader class
    * ID3D11PixelShader* m_pPixelShader = nullptr;
    */
    

    /**
    * @brief pointer to direct 11 Blob class
    * ID3DBlob* m_pPSBlob = nullptr;
    */
    

    friend class D3D12GraphicsApi;
  };
}