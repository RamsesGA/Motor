#pragma once

#include <d3d11.h>
#include <gaIndexBuffer.h>

namespace gaEngineSDK {
  class IndexBufferDX final : public IndexBuffer
  {
   public:
    /*************************************************************************/
    /**
    * Constructor and destructor.
    */
    /*************************************************************************/
    IndexBufferDX() = default;

    ~IndexBufferDX() = default;

   protected:
    /*************************************************************************/
    /**
    * Members.
    */
    /*************************************************************************/

    /**
    * @brief DX member to save the buffer.
    */
    ID3D11Buffer* m_pIndexBuffer = nullptr;

    friend class GraphicsApiDX;
  };
}