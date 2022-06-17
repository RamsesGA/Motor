#pragma once

#include <gaVertexBuffer.h>

namespace gaEngineSDK {
  class VertexBufferDX final : public VertexBuffer
  {
  public:
    /*************************************************************************/
    /**
    * Constructor and destructor.
    */
    /*************************************************************************/
    VertexBufferDX() = default;

    ~VertexBufferDX() {
      SAFE_RELEASE(m_pVertexBuffer);
    };

  protected:
    /*************************************************************************/
    /**
    * Members.
    */
    /*************************************************************************/

    /**
    * @brief DX member to save the buffer.
    */
    ID3D11Buffer* m_pVertexBuffer = nullptr;

    friend class GraphicsApiDX;
  };
}