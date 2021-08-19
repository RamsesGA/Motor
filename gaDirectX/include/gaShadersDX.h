#pragma once

#include <d3d11.h>
#include <gaShaders.h>

namespace gaEngineSDK {
  class ShadersDX final : public Shaders
  {
   public:
    /*************************************************************************/
    /**
    * Constructor and destructor.
    */
    /*************************************************************************/
    ShadersDX() = default;

    ~ShadersDX() = default;

   protected:
    /*************************************************************************/
    /**
    * Members.
    */
    /*************************************************************************/

    /**
    * @brief DX member to save the vertex shader.
    */
    ID3D11VertexShader* m_pVertexShader;

    /**
    * @brief DX member to save the blob.
    */
    ID3DBlob* m_pVSBlob;

    /**
    * @brief DX member to save the pixel shader.
    */
    ID3D11PixelShader* m_pPixelShader;

    /**
    * @brief DX member to save the blob.
    */
    ID3DBlob* m_pPSBlob;

    friend class GraphicsApiDX;
  };
}