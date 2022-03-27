#pragma once

#include <d3d12.h>

#include <gaSamplerState.h>

namespace gaEngineSDK {
  class D3D12SamplerState final : public SamplerState
  {
  public:
    /*************************************************************************/
    /**
    * Constructor and destructor.
    */
    /*************************************************************************/
    D3D12SamplerState() = default;

    ~D3D12SamplerState() = default;

  protected:
    /*************************************************************************/
    /**
    * Members.
    */
    /*************************************************************************/

    /**
    * @brief DX member to save the sampler state.
    * ID3D11SamplerState* m_pSamplerState = nullptr;
    */
    

    friend class D3D12GraphicsApi;
  };
}