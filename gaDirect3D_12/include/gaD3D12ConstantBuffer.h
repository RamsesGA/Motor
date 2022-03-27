#pragma once

#include <d3d12.h>

#include <gaConstantBuffer.h>

namespace gaEngineSDK {
  class D3D12ConstantBuffer final : public ConstantBuffer
  {
  public:
    /*************************************************************************/
    /**
    * Constructor and destructor.
    */
    /*************************************************************************/
    D3D12ConstantBuffer() = default;

    ~D3D12ConstantBuffer() = default;

  protected:
    /*************************************************************************/
    /**
    * Members
    */
    /*************************************************************************/

    /**
    * @brief DX member to save the buffer.
    * ID3D11Buffer* m_pConstantBuffer = nullptr;
    */
    

    friend class D3D12GraphicsApi;
  };
}