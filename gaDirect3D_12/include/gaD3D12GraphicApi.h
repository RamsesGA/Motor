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

#include <gaGraphicsApi.h>
#include <SFML/Window.hpp>

#include <cassert>

#include "d3dx12.h"
#include "gaD3D12Prerequisites.h"
#include "gaStructuresRayTracing.h"


namespace gaEngineSDK {
  using std::cout;
  using std::make_unique;
  using sf::WindowHandle;

  using namespace Microsoft::WRL;

  interface IDeviceNotify {
    virtual void 
    onDeviceLost() = 0;

    virtual void 
    onDeviceRestored() = 0;
  };

  class D3D12GraphicApi final : public GraphicsApi
  {
  public:
    /*************************************************************************/
    /**
    * Constructor and destructor.
    */
    /*************************************************************************/
    
    //D3D12GraphicApi() = default;

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
    initializeDXGIAdapter();

    void
    initializeAdapter(IDXGIAdapter1** ppAdapter);

    void
    swapChainPresent(uint32 syncInterval = 0, uint32 flags = 0) override;

    void
    clear(WeakSPtr<RenderTarget> pRt,
          const float& r,
          const float& g,
          const float& b,
          const float& a) override;

    void 
    executeCommandList();

    void 
    waitForGpu() noexcept;

    void 
    handleDeviceLost();

    void
    moveToNextFrame();

    bool
    isDXRaytracingSupported(IDXGIAdapter1* adapter);
    
    bool
    createSwapChain();

    bool
    createRtvAndDsvDescriptorHeaps();

    void
    createRenderTargets();

    bool
    createDepthStencil();

    void
    shutdown();

    bool 
    isTearingSupported() const { 
      return m_options & c_AllowTearing; 
    }

    template<typename T>
    constexpr void release(T*& resource);

    /*************************************************************************/
    /**
    * Gets.
    */
    /*************************************************************************/

    void
    getHardwareAdapter(IDXGIFactory1* pFactory1,
                       IDXGIAdapter1** ppAdapter1,
                       bool requestHighPerformaAdapter);

    /*************************************************************************/
    /**
    * Sets.
    */
    /*************************************************************************/

    void
    setRenderTarget(WeakSPtr<RenderTarget> pRt);

    void
    setViewports(const uint32 width, 
                 const uint32 heigth, 
                 const uint32 numViewports = 1) override;

  private:
    /*************************************************************************/
    /**
    * Members.
    */
    /*************************************************************************/

    const static size_t MAX_BACK_BUFFER_COUNT = 3;

    static const unsigned int c_AllowTearing = 0x1;

    static const unsigned int c_RequireTearingSupport = 0x2;

    uint32 m_frameIndex = 0;

    uint32 m_backBufferIndex;

    uint32 m_rtvDescriptorSize;

    uint32 m_backBufferCount = 3;

    uint32 m_m4xMsaaQuality = 0;

    uint32 m_options = 0;

    uint32 m_adapterIDoverride;

    uint32 m_adapterID;

    uint64 m_fenceValues[MAX_BACK_BUFFER_COUNT];

    WString m_adapterDescription;

    RECT m_outputSize;

    WindowHandle m_hWnd;

    DXGI_FORMAT m_backBufferFormat = DXGI_FORMAT_B8G8R8A8_UNORM;

    DXGI_FORMAT m_depthBufferFormat = DXGI_FORMAT_D32_FLOAT;

    D3D_FEATURE_LEVEL m_d3dMinFeatureLevel = D3D_FEATURE_LEVEL_11_0;

    D3D_FEATURE_LEVEL m_d3dFeatureLevel;

    ComPtr<IDXGIAdapter1> m_pAdapter;

    Microsoft::WRL::ComPtr<ID3D12Device> m_d3dDevice;

    Microsoft::WRL::ComPtr<ID3D12Fence> m_fence;

    Microsoft::WRL::ComPtr<IDXGIFactory4> m_dxgiFactory4;

    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_dsvDescriptorHeap;

    Microsoft::WRL::ComPtr<ID3D12CommandQueue> m_commandQueue;

    Microsoft::WRL::ComPtr<ID3D12CommandAllocator> m_commandAllocators[MAX_BACK_BUFFER_COUNT];

    Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> m_commandList;

    Microsoft::WRL::ComPtr<IDXGISwapChain3> m_swapChain;

    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_rtvDescriptorHeap;

    Microsoft::WRL::ComPtr<ID3D12Resource> m_renderTargets[MAX_BACK_BUFFER_COUNT];

    Microsoft::WRL::ComPtr<ID3D12Resource> m_depthStencil;

    Microsoft::WRL::Wrappers::Event m_fenceEvent;

    SPtr<RenderTarget> m_pRenderTarget;

    IDeviceNotify* m_deviceNotify;

    D3D12_CPU_DESCRIPTOR_HANDLE m_pRtvStart;

    D3D12_VIEWPORT m_screenViewport;
  };

  /***************************************************************************/
  /**
  * Export.
  */
  /***************************************************************************/

  extern "C" GA_D3D12_EXPORT D3D12GraphicApi *
  createGraphicApiD3D12() {
    D3D12GraphicApi* pD3D12GraphicsApi = new D3D12GraphicApi();
    return pD3D12GraphicsApi;
  }
}