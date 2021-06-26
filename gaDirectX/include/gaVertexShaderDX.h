#pragma once

#include <d3d11.h>
#include <gaVertexShader.h>

namespace gaEngineSDK {
  class VertexShaderDX final : public VertexShader
  {
    public:
      /***********************************************************************/
      /**
      * Constructor and destructor.
      */
      /***********************************************************************/
      VertexShaderDX() = default;
    
      /**
       * @brief Default destructor
       */
      ~VertexShaderDX() = default;
    
    protected:
      /**
       * @brief pointer to direct 11 VertexShader class
       */
      ID3D11VertexShader* m_pVertexShader = nullptr;
    
      /**
       * @brief pointer to direct 11 Blob class
       */
      ID3DBlob* m_pVSBlob = nullptr;

      friend class GraphicsApiDX;
  };
}