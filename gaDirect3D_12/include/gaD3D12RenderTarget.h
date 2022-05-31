#pragma once

#include <dxgi.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <d3dcommon.h>
#include <dxgiformat.h>
#include <d3dcompiler.h>
#include <dxgidebug.h>
#include <Windows.Foundation.h>
#include <wrl\wrappers\corewrappers.h>
#include <wrl\client.h>

#include "gaRenderTarget.h"

namespace gaEngineSDK {
  using namespace Microsoft::WRL;

  class D3D12RenderTarget final : public RenderTarget
  {
  public:
    /*************************************************************************/
    /**
    * Constructor and destructor.
    */
    /*************************************************************************/
    D3D12RenderTarget() = default;

    ~D3D12RenderTarget() = default;

    /*************************************************************************/
    /**
    * Methods.
    */
    /*************************************************************************/

    /*
    * @brief .
    */
    void*
    getRenderTexture(uint32 index = 0) override { };

    void*
    getDepthStencilView() override { }

  protected:
    /*************************************************************************/
    /**
    * Members.
    */
    /*************************************************************************/

    const static size_t MAX_BACK_BUFFER_COUNT = 3;

    Microsoft::WRL::ComPtr<ID3D12Resource> m_renderTargets[MAX_BACK_BUFFER_COUNT];

    friend class D3D12GraphicsApi;
  };
}