#pragma once

#include <d3d11.h>
#include <d3dcompiler.h>
#include <d3d11shader.h>
#include <gaPrerequisitesCore.h>
#include <gaComputeBuffer.h>

namespace gaEngineSDK {
  class ComputeBufferDX final : public ComputeBuffer
  {
  public:
    /*************************************************************************/
    /**
    * Constructor and destructor.
    */
    /*************************************************************************/
    ComputeBufferDX() = default;

    ~ComputeBufferDX() {
      SAFE_RELEASE(m_pComputeBuffer);
      SAFE_RELEASE(m_pComputeUAV);

      if (0 != m_vpComputeSRV.size()) {
        uint32 tempSize = m_vpComputeSRV.size();
        for (uint32 i = 0; i < tempSize; ++i) {
          SAFE_RELEASE(m_vpComputeSRV[i]);
        }
      }
    }

  protected:
    /*************************************************************************/
    /**
    * Members.
    */
    /*************************************************************************/

    /*
    * @brief .
    */
    uint32 m_offset = 0;

    /*
    * @brief .
    */
    ID3D11Buffer* m_pComputeBuffer = nullptr;

    /*
    * @brief .
    */
    Vector<ID3D11ShaderResourceView*> m_vpComputeSRV;

    /*
    * @brief .
    */
    ID3D11UnorderedAccessView* m_pComputeUAV = nullptr;

    friend class GraphicsApiDX;
  };
}