#pragma once

#include <d3d11.h>
#include <gaShaders.h>

#include "gaComputeShaderDX.h"

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
    ID3D11VertexShader* m_pVertexShader = nullptr;

    /**
    * @brief DX member to save the blob.
    */
    ID3DBlob* m_pVSBlob = nullptr;

    /**
    * @brief DX member to save the pixel shader.
    */
    ID3D11PixelShader* m_pPixelShader = nullptr;

    /**
    * @brief DX member to save the blob.
    */
    ID3DBlob* m_pPSBlob = nullptr;

    /*
    * @brief .
    */
    SPtr<ComputeShaderDX> m_pComputeShader;

    friend class GraphicsApiDX;
  };
}