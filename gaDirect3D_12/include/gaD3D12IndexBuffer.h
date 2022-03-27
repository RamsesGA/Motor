#pragma once

#include <d3d12.h>

#include <gaIndexBuffer.h>

namespace gaEngineSDK {
  class D3D12IndexBuffer final : public IndexBuffer
  {
  public:
    /*************************************************************************/
    /**
    * Constructor and destructor.
    */
    /*************************************************************************/
    D3D12IndexBuffer() = default;

    ~D3D12IndexBuffer() = default;

  protected:
    /*************************************************************************/
    /**
    * Members.
    */
    /*************************************************************************/

    /**
    * @brief DX member to save the buffer.
    * ID3D11Buffer* m_pIndexBuffer = nullptr;
    */
    

    friend class D3D12GraphicsApi;
  };
}