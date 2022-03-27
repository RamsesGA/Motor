#pragma once

#include <gaVertexBuffer.h>

namespace gaEngineSDK {
  class D3D12VertexBuffer final : public VertexBuffer
  {
  public:
    /*************************************************************************/
    /**
    * Constructor and destructor.
    */
    /*************************************************************************/
    D3D12VertexBuffer() = default;

    ~D3D12VertexBuffer() = default;

  protected:
    /*************************************************************************/
    /**
    * Members.
    */
    /*************************************************************************/

    /**
    * @brief DX member to save the buffer.
    * ID3D11Buffer* m_pVertexBuffer = nullptr;
    */

    friend class D3D12GraphicsApi;
  };
}