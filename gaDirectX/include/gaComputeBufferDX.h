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

    ~ComputeBufferDX() = default;

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