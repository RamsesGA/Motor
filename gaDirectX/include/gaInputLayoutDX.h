#pragma once

#include <d3d11.h>
#include <gaInputLayout.h>

namespace gaEngineSDK {
  class InputLayoutDX final : public InputLayout
  {
  public:
    /*************************************************************************/
    /**
    * Constructor and destructor.
    */
    /*************************************************************************/
    InputLayoutDX() = default;

    ~InputLayoutDX() {
      if (0 != m_inputLayoutByteLength) {
        m_inputLayoutByteLength = 0;
      }
      SAFE_RELEASE(m_pVertexLayout);
    };

  protected:
    /*************************************************************************/
    /**
    * Members.
    */
    /*************************************************************************/

    /**
    * @brief DX member to save byte off set.
    */
    uint32* m_inputLayoutByteLength = 0;

    /**
    * @brief DX member to save the vertex layout.
    */
    ID3D11InputLayout* m_pVertexLayout = nullptr;

    friend class GraphicsApiDX;
  };
}