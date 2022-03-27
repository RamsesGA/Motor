#pragma once

//#include <d3d12.h>
#include <dxgi1_6.h>
#include <d3dcompiler.h>
#include <Windows.Foundation.h>
#include <wrl\wrappers\corewrappers.h>
#include <wrl\client.h>

#include <gaGraphicsApi.h>
#include <SFML/Window.hpp>

#include "gaD3D12Prerequisites.h"
#include "gaStructuresRayTracing.h"
#include "gaD3D12DeviceResources.h"

namespace gaEngineSDK {
  using std::cout;
  using std::make_unique;
  using namespace Microsoft::WRL;

  class D3D12GraphicApi final : public GraphicsApi, public IDeviceNotify
  {
  public:
    /*************************************************************************/
    /**
    * Constructor and destructor.
    */
    /*************************************************************************/

    D3D12GraphicApi() = default;

    ~D3D12GraphicApi() = default;

    /*************************************************************************/
    /**
    * Methods.
    */
    /*************************************************************************/

    bool
    initDevice(sf::WindowHandle hWnd) override;

    void
    shutdown();

    template<typename T>
    constexpr void release(T*& resource);

  private:
    /*************************************************************************/
    /**
    * Members.
    */
    /*************************************************************************/

    /*
    static const uint32 m_frameCount = 3;

    uint32 m_adapterIDoverride;

    SPtr<D3D12DeviceResource> m_deviceResources;

    RayGenConstantBuffer m_rayGenCB;

    ID3D12Device8* m_pMainDevice = nullptr;

    IDXGIFactory7* m_pDxgiFactory = nullptr;
    */

    ComPtr<IDXGIAdapter1> m_pAdapter;

    D3D_FEATURE_LEVEL m_d3dMinFeatureLevel = D3D_FEATURE_LEVEL_11_0;

    Microsoft::WRL::ComPtr<ID3D12Device> m_d3dDevice;

    Microsoft::WRL::ComPtr<ID3D12Fence> m_fence;
  };

  /***************************************************************************/
  /**
  * Export.
  */
  /***************************************************************************/

  extern "C" GA_D3D12_EXPORT D3D12GraphicApi *
  createGraphicApiD3D12() {
    D3D12GraphicApi* pGraphicsApiD3D12 = new D3D12GraphicApi();
    return pGraphicsApiD3D12;
  }
}