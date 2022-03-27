#pragma once

#include <d3d12.h>

#include <gaVertexShader.h>

namespace gaEngineSDK {
  class D3D12VertexShader final : public VertexShader
  {
  public:
    /*************************************************************************/
    /**
    * Constructor and destructor.
    */
    /*************************************************************************/
    D3D12VertexShader() = default;

    ~D3D12VertexShader() = default;

  protected:
    /*************************************************************************/
    /**
    * Members.
    */
    /*************************************************************************/

    /**
    * @brief pointer to direct 11 VertexShader class
    * ID3D11VertexShader* m_pVertexShader = nullptr;
    */

    /**
    * @brief pointer to direct 11 Blob class
    * ID3DBlob* m_pVSBlob = nullptr;
    */

    friend class D3D12GraphicsApi;
  };
}