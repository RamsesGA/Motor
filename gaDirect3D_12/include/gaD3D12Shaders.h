#pragma once

#include <d3d12.h>

#include <gaShaders.h>

namespace gaEngineSDK {
  class D3D12Shaders final : public Shaders
  {
  public:
    /*************************************************************************/
    /**
    * Constructor and destructor.
    */
    /*************************************************************************/
    D3D12Shaders() = default;

    ~D3D12Shaders() = default;

  protected:
    /*************************************************************************/
    /**
    * Members.
    */
    /*************************************************************************/

    /**
    * @brief DX member to save the vertex shader.
    * ID3D11VertexShader* m_pVertexShader = nullptr;
    */
    

    /**
    * @brief DX member to save the blob.
    * ID3DBlob* m_pVSBlob = nullptr;
    */
    

    /**
    * @brief DX member to save the pixel shader.
    * ID3D11PixelShader* m_pPixelShader = nullptr;
    */
    

    /**
    * @brief DX member to save the blob.
    * ID3DBlob* m_pPSBlob = nullptr;
    */
    

    /*
    * @brief .
    * SPtr<ComputeShaderDX> m_pComputeShader;
    */
    

    friend class D3D12GraphicsApi;
  };
}