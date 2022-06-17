#pragma once

#include <d3d11.h>
#include <gaPixelShader.h>

namespace gaEngineSDK {
  class PixelShaderDX final : public PixelShader
  {
  public:
    /*************************************************************************/
    /**
    * Constructor and destructor.
    */
    /*************************************************************************/
    PixelShaderDX() = default;

    ~PixelShaderDX() {
      SAFE_RELEASE(m_pPixelShader);
      SAFE_RELEASE(m_pPSBlob);
    };

  protected:
    /*************************************************************************/
    /**
    * Members.
    */
    /*************************************************************************/

    /**
    * @brief pointer to direct 11 pixel shader class
    */
    ID3D11PixelShader* m_pPixelShader = nullptr;

    /**
    * @brief pointer to direct 11 Blob class
    */
    ID3DBlob* m_pPSBlob = nullptr;

    friend class GraphicsApiDX;
  };
}