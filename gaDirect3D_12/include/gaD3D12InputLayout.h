#pragma once

#include <d3d12.h>

#include <gaInputLayout.h>

namespace gaEngineSDK {
  class D3D12InputLayout final : public InputLayout
  {
  public:
    /*************************************************************************/
    /**
    * Constructor and destructor.
    */
    /*************************************************************************/
    D3D12InputLayout() = default;

    ~D3D12InputLayout() = default;

  protected:
    /*************************************************************************/
    /**
    * Members.
    */
    /*************************************************************************/

    /**
    * @brief DX member to save byte off set.
    * uint32* m_inputLayoutByteLength = 0;
    */
    

    /**
    * @brief DX member to save the vertex layout.
    * ID3D11InputLayout* m_pVertexLayout = nullptr;
    */
    

    friend class D3D12GraphicsApi;
  };
}