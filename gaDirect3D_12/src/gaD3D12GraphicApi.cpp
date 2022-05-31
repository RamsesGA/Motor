#include <exception>

#include "gaD3D12GraphicApi.h"
#include "gaHelper.h"

namespace gaEngineSDK {
  namespace {
    inline DXGI_FORMAT NoSRGB(DXGI_FORMAT fmt) {
      switch (fmt) {
        case DXGI_FORMAT_R8G8B8A8_UNORM_SRGB:   
          return DXGI_FORMAT_R8G8B8A8_UNORM;

        case DXGI_FORMAT_B8G8R8A8_UNORM_SRGB:   
          return DXGI_FORMAT_B8G8R8A8_UNORM;

        case DXGI_FORMAT_B8G8R8X8_UNORM_SRGB:   
          return DXGI_FORMAT_B8G8R8X8_UNORM;

        default:
          return fmt;
      }
    }
  };

  /***************************************************************************/
  /**
  * Methods.
  */
  /***************************************************************************/

  bool 
  D3D12GraphicApi::initDevice(sf::WindowHandle hWnd) {
    initializeDXGIAdapter();

    ThrowIfFalse(isDXRaytracingSupported(m_pAdapter.Get()),
       L"ERROR: DirectX Raytracing is not supported by your OS, GPU and/or driver.\n\n");

    //Hardware result.
    HRESULT hr;

    m_hWnd = hWnd;

    // Create the DX12 API device object.
    ThrowIfFailed(D3D12CreateDevice(m_pAdapter.Get(), 
                                    m_d3dMinFeatureLevel,
                                    IID_PPV_ARGS(&m_d3dDevice)));
#ifndef NDEBUG
    // Configure debug device (if active).
    ComPtr<ID3D12InfoQueue> d3dInfoQueue;
    if (SUCCEEDED(m_d3dDevice.As(&d3dInfoQueue))) {
#ifdef _DEBUG
      d3dInfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, true);
      d3dInfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, true);
#endif
      D3D12_MESSAGE_ID hide[] =
      {
        D3D12_MESSAGE_ID_MAP_INVALID_NULLRANGE,
        D3D12_MESSAGE_ID_UNMAP_INVALID_NULLRANGE
      };
      D3D12_INFO_QUEUE_FILTER filter = {};

      filter.DenyList.NumIDs = _countof(hide);
      filter.DenyList.pIDList = hide;
      d3dInfoQueue->AddStorageFilterEntries(&filter);
    }
#endif

    // Determine maximum supported feature level for this device
    static const D3D_FEATURE_LEVEL s_featureLevels[] =
    {
      D3D_FEATURE_LEVEL_12_1,
      D3D_FEATURE_LEVEL_12_0,
      D3D_FEATURE_LEVEL_11_1,
      D3D_FEATURE_LEVEL_11_0,
    };

    D3D12_FEATURE_DATA_FEATURE_LEVELS featLevels =
    {
      _countof(s_featureLevels), s_featureLevels, D3D_FEATURE_LEVEL_11_0
    };

    hr = m_d3dDevice->CheckFeatureSupport(D3D12_FEATURE_FEATURE_LEVELS, &featLevels, sizeof(featLevels));
    if (SUCCEEDED(hr)) {
      m_d3dFeatureLevel = featLevels.MaxSupportedFeatureLevel;
    }
    else {
      m_d3dFeatureLevel = m_d3dMinFeatureLevel;
    }

    //Create the command queue.
    D3D12_COMMAND_QUEUE_DESC queueDesc = {};
    queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
    queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

    ThrowIfFailed(m_d3dDevice->CreateCommandQueue(&queueDesc, 
                                                  IID_PPV_ARGS(&m_commandQueue)));

    createRtvAndDsvDescriptorHeaps();

    // Create a command allocator for each back buffer that will be rendered to.
    // Create a command allocator for each back buffer that will be rendered to.
    for (uint32 n = 0; n < m_backBufferCount; ++n) {
      ThrowIfFailed(m_d3dDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT,
                                                        IID_PPV_ARGS(m_commandAllocators[n].GetAddressOf())));
    }

    // Create a command list for recording graphics commands.
    ThrowIfFailed(m_d3dDevice->CreateCommandList(0, 
                                                 D3D12_COMMAND_LIST_TYPE_DIRECT, 
                                                 m_commandAllocators[0].Get(), 
                                                 nullptr, 
                                                 IID_PPV_ARGS(m_commandList.GetAddressOf())));

    //ThrowIfFailed(m_commandList->Close());

    // Create a fence for tracking GPU execution progress.
    ThrowIfFailed(m_d3dDevice->CreateFence(m_fenceValues[m_backBufferIndex], 
                                           D3D12_FENCE_FLAG_NONE, 
                                           IID_PPV_ARGS(&m_fence)));
    m_fenceValues[m_backBufferIndex]++;

    m_fenceEvent.Attach(CreateEvent(nullptr, false, false, nullptr));

    if (!m_fenceEvent.IsValid()) {
      ThrowIfFailed(E_FAIL, L"CreateEvent failed.\n");
    }

    /*
    * 
    * 
    * 
    */

    m_rtvDescriptorSize = m_d3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

    D3D12_DESCRIPTOR_HEAP_DESC dsvDescriptorHeapDesc = {};
    dsvDescriptorHeapDesc.NumDescriptors = 1;
    dsvDescriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;

    hr = m_d3dDevice->CreateDescriptorHeap(&dsvDescriptorHeapDesc,
                                           IID_PPV_ARGS(&m_dsvDescriptorHeap));
    if (FAILED(hr)) {
      cout << "\nError, failed CreateDescriptorHeap\n";
      return false;
    }

    //Check 4X MSAA Quality Support
    D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS msQualityLevels;
    msQualityLevels.Format = m_backBufferFormat;
    msQualityLevels.SampleCount = 4;
    msQualityLevels.Flags = D3D12_MULTISAMPLE_QUALITY_LEVELS_FLAG_NONE;
    msQualityLevels.NumQualityLevels = 0;

    hr = m_d3dDevice->CheckFeatureSupport(D3D12_FEATURE_MULTISAMPLE_QUALITY_LEVELS,
                                          &msQualityLevels,
                                          sizeof(msQualityLevels));
    if (FAILED(hr)) {
      cout << "\nError, failed CheckFeatureSupport\n";
      return false;
    }

    m_m4xMsaaQuality = msQualityLevels.NumQualityLevels;

    //The returned quality should always be greater than 0
    assert(m_m4xMsaaQuality > 0 && "Unexpected MSAA quality level.");

    createSwapChain();

    createRenderTargets();

    createDepthStencil();

    return true;
  }
  
  void 
  D3D12GraphicApi::swapChainPresent(uint32 syncInterval, uint32 flags) {
    ThrowIfFailed(m_commandList->Close());

    ID3D12CommandList* cmdList[] = { m_commandList.Get() };

    m_commandQueue->ExecuteCommandLists(_countof(cmdList), cmdList);

    ThrowIfFailed(m_swapChain->Present(0, 0));

    waitForGpu();

    //ThrowIfFailed(m_commandAllocators[0]->Reset());

    ThrowIfFailed(m_commandList->Reset(m_commandAllocators[0].Get(), nullptr));


    m_frameIndex = m_swapChain->GetCurrentBackBufferIndex();
  }

  void 
  D3D12GraphicApi::clear(WeakSPtr<RenderTarget> pRt, 
                         const float& r, 
                         const float& g, 
                         const float& b,
                         const float& a) {
    float color[4] = { r,g,b,a };

    auto res = CD3DX12_RESOURCE_BARRIER::Transition(m_renderTargets[m_frameIndex].Get(), 
                                                    D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
    
    m_commandList->ResourceBarrier(1, &res);

    CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_pRtvStart, m_frameIndex, m_rtvDescriptorSize);

    m_commandList->ClearRenderTargetView(rtvHandle, color, 0, nullptr);
  }

  void
  D3D12GraphicApi::executeCommandList() {
    ThrowIfFailed(m_commandList->Close());

    ID3D12CommandList* commandLists[] = { m_commandList.Get() };

    m_commandQueue->ExecuteCommandLists(ARRAYSIZE(commandLists), commandLists);
  }

  void 
  D3D12GraphicApi::waitForGpu() noexcept {
    if (m_commandQueue && m_fence && m_fenceEvent.IsValid()) {

      // Schedule a Signal command in the GPU queue.
      UINT64 fenceValue = m_fenceValues[m_backBufferIndex];
      if (SUCCEEDED(m_commandQueue->Signal(m_fence.Get(), fenceValue))) {

        // Wait until the Signal has been processed.
        if (SUCCEEDED(m_fence->SetEventOnCompletion(fenceValue, m_fenceEvent.Get()))) {
          WaitForSingleObjectEx(m_fenceEvent.Get(), INFINITE, FALSE);

          // Increment the fence value for the current frame.
          m_fenceValues[m_backBufferIndex]++;
        }
      }
    }
  }

  void 
  D3D12GraphicApi::handleDeviceLost() {
    if (m_deviceNotify) {
      m_deviceNotify->onDeviceLost();
    }

    for (UINT n = 0; n < m_backBufferCount; ++n) {
      m_commandAllocators[n].Reset();
      m_renderTargets[n].Reset();
    }

    m_depthStencil.Reset();
    m_commandQueue.Reset();
    m_commandList.Reset();
    m_fence.Reset();
    m_rtvDescriptorHeap.Reset();
    m_dsvDescriptorHeap.Reset();
    m_swapChain.Reset();
    m_d3dDevice.Reset();
    m_dxgiFactory4.Reset();
    m_pAdapter.Reset();

#ifdef _DEBUG
    {
      ComPtr<IDXGIDebug1> dxgiDebug;
      if (SUCCEEDED(DXGIGetDebugInterface1(0, IID_PPV_ARGS(&dxgiDebug)))) {
        dxgiDebug->ReportLiveObjects(DXGI_DEBUG_ALL, 
                                     DXGI_DEBUG_RLO_FLAGS(DXGI_DEBUG_RLO_SUMMARY | 
                                                          DXGI_DEBUG_RLO_IGNORE_INTERNAL));
      }
    }
#endif
    initializeDXGIAdapter();

    if (m_deviceNotify) {
        m_deviceNotify->onDeviceRestored();
    }
  }

  void
  D3D12GraphicApi::moveToNextFrame() {
    // Schedule a Signal command in the queue.
    const UINT64 currentFenceValue = m_fenceValues[m_backBufferIndex];

    ThrowIfFailed(m_commandQueue->Signal(m_fence.Get(), currentFenceValue));

    // Update the back buffer index.
    m_backBufferIndex = m_swapChain->GetCurrentBackBufferIndex();

    // If the next frame is not ready to be rendered yet, wait until it is ready.
    if (m_fence->GetCompletedValue() < m_fenceValues[m_backBufferIndex]) {
      ThrowIfFailed(m_fence->SetEventOnCompletion(m_fenceValues[m_backBufferIndex], 
                                                  m_fenceEvent.Get()));

      WaitForSingleObjectEx(m_fenceEvent.Get(), INFINITE, FALSE);
    }

    // Set the fence value for the next frame.
    m_fenceValues[m_backBufferIndex] = currentFenceValue + 1;
  }

  bool 
  D3D12GraphicApi::isDXRaytracingSupported(IDXGIAdapter1* adapter) {
    ComPtr<ID3D12Device> testDev;

    D3D12_FEATURE_DATA_D3D12_OPTIONS5 featureSuppData = {};

    return SUCCEEDED(D3D12CreateDevice(adapter,
                                       D3D_FEATURE_LEVEL_11_0,
                                       IID_PPV_ARGS(&testDev))) &&
           SUCCEEDED(testDev->CheckFeatureSupport(D3D12_FEATURE_D3D12_OPTIONS5,
                                                  &featureSuppData,
                                                  sizeof(featureSuppData))) &&
           featureSuppData.RaytracingTier != D3D12_RAYTRACING_TIER_NOT_SUPPORTED;
  }

  void
  D3D12GraphicApi::initializeDXGIAdapter() {
    bool debugDXGI = false;

    m_outputSize.left = m_outputSize.top = 0;
    m_outputSize.right = m_width;
    m_outputSize.bottom = m_height;

#if defined(_DEBUG)
    {
      ComPtr<ID3D12Debug> debugController;
      if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
      {
        debugController->EnableDebugLayer();
      }
      else
      {
        OutputDebugStringA("WARNING: Direct3D Debug Device is not available\n");
      }

      ComPtr<IDXGIInfoQueue> dxgiInfoQueue;
      if (SUCCEEDED(DXGIGetDebugInterface1(0, IID_PPV_ARGS(&dxgiInfoQueue))))
      {
        debugDXGI = true;

        ThrowIfFailed(CreateDXGIFactory2(DXGI_CREATE_FACTORY_DEBUG, IID_PPV_ARGS(&m_dxgiFactory4)));

        dxgiInfoQueue->SetBreakOnSeverity(DXGI_DEBUG_ALL, DXGI_INFO_QUEUE_MESSAGE_SEVERITY_ERROR, true);
        dxgiInfoQueue->SetBreakOnSeverity(DXGI_DEBUG_ALL, DXGI_INFO_QUEUE_MESSAGE_SEVERITY_CORRUPTION, true);
      }
    }
#endif

    if (!debugDXGI) {
      ThrowIfFailed(CreateDXGIFactory1(IID_PPV_ARGS(&m_dxgiFactory4)));
    }

    if (m_options & (c_AllowTearing | c_RequireTearingSupport)) {
      BOOL allowTearing = FALSE;

      ComPtr<IDXGIFactory5> factory5;
      HRESULT hr = m_dxgiFactory4.As(&factory5);

      if (SUCCEEDED(hr)) {
        hr = factory5->CheckFeatureSupport(DXGI_FEATURE_PRESENT_ALLOW_TEARING, &allowTearing, sizeof(allowTearing));
      }

      if (FAILED(hr) || !allowTearing) {
        OutputDebugStringA("WARNING: Variable refresh rate displays are not supported.\n");

        if (m_options & c_RequireTearingSupport) {
          ThrowIfFailed(false, 
                        L"Error: Sample must be run on an OS with tearing support.\n");
        }

        m_options &= ~c_AllowTearing;
      }
    }

    initializeAdapter(&m_pAdapter);
  }

  void
  D3D12GraphicApi::initializeAdapter(IDXGIAdapter1** ppAdapter) {
    *ppAdapter = nullptr;

    ComPtr<IDXGIAdapter1> adapter;
    ComPtr<IDXGIFactory6> factory6;

    HRESULT hr = m_dxgiFactory4.As(&factory6);

    if (FAILED(hr)) {
      throw std::exception("DXGI 1.6 not supported");
    }
    for (UINT adapterID = 0; 
         DXGI_ERROR_NOT_FOUND != factory6->EnumAdapterByGpuPreference(adapterID, 
                                                                      DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE, 
                                                                      IID_PPV_ARGS(&adapter)); 
         ++adapterID) {

      if (m_adapterIDoverride != UINT_MAX && adapterID != m_adapterIDoverride) {
        continue;
      }

      DXGI_ADAPTER_DESC1 desc;
      ThrowIfFailed(adapter->GetDesc1(&desc));

      if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) {
        // Don't select the Basic Render Driver adapter.
        continue;
      }

      // Check to see if the adapter supports Direct3D 12, but don't create the actual device yet.
      if (SUCCEEDED(D3D12CreateDevice(adapter.Get(), 
                                      m_d3dMinFeatureLevel, 
                                      __uuidof(ID3D12Device), 
                                      nullptr))) {
        m_adapterID = adapterID;
        m_adapterDescription = desc.Description;

#ifdef _DEBUG
        wchar_t buff[256] = {};
        swprintf_s(buff, 
                   L"Direct3D Adapter (%u): VID:%04X, PID:%04X - %ls\n", 
                   adapterID, 
                   desc.VendorId, 
                   desc.DeviceId, 
                   desc.Description);

        OutputDebugStringW(buff);
#endif
        break;
      }
    }

#if !defined(NDEBUG)
    if (!adapter && m_adapterIDoverride == UINT_MAX) {
      // Try WARP12 instead
      if (FAILED(m_dxgiFactory4->EnumWarpAdapter(IID_PPV_ARGS(&adapter)))) {
        throw std::exception("WARP12 not available. Enable the 'Graphics Tools' optional feature");
      }

      OutputDebugStringA("Direct3D Adapter - WARP12\n");
    }
#endif

    if (!adapter) {
      if (m_adapterIDoverride != UINT_MAX) {
        throw std::exception("Unavailable adapter requested.");
      }
      else {
        throw std::exception("Unavailable adapter.");
      }
    }

    *ppAdapter = adapter.Detach();
  }

  bool
  D3D12GraphicApi::createSwapChain() {
    waitForGpu();

    m_swapChain.Reset();

    HRESULT hr;

    //Determine the render target size in pixels.
    UINT backBufferWidth = max(m_outputSize.right - m_outputSize.left, 1);
    UINT backBufferHeight = max(m_outputSize.bottom - m_outputSize.top, 1);
    DXGI_FORMAT backBufferFormat = NoSRGB(m_backBufferFormat);

    DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
    swapChainDesc.Width = backBufferWidth;
    swapChainDesc.Height = backBufferHeight;
    swapChainDesc.Format = backBufferFormat;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.BufferCount = m_backBufferCount;
    swapChainDesc.SampleDesc.Count = 1;
    swapChainDesc.SampleDesc.Quality = 0;
    swapChainDesc.Scaling = DXGI_SCALING_STRETCH;
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    swapChainDesc.AlphaMode = DXGI_ALPHA_MODE_IGNORE;
    swapChainDesc.Flags = (m_options & c_AllowTearing) ? DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING : 0;

    DXGI_SWAP_CHAIN_FULLSCREEN_DESC fsSwapChainDesc = { 0 };
    fsSwapChainDesc.Windowed = true;

    //Create a swap chain for the window.
    ComPtr<IDXGISwapChain1> swapChain;

    ThrowIfFailed(m_dxgiFactory4->CreateSwapChainForHwnd(m_commandQueue.Get(),
                                                         m_hWnd, 
                                                         &swapChainDesc, 
                                                         &fsSwapChainDesc, 
                                                         nullptr, 
                                                         &swapChain));
    ThrowIfFailed(swapChain.As(&m_swapChain));

    if (isTearingSupported()) {
      m_dxgiFactory4->MakeWindowAssociation(m_hWnd, DXGI_MWA_NO_ALT_ENTER);
    }

    waitForGpu();

    return true;
  }

  bool
  D3D12GraphicApi::createRtvAndDsvDescriptorHeaps() {
    HRESULT hr;

    //Create descriptor heaps for render target views and depth stencil views.
    D3D12_DESCRIPTOR_HEAP_DESC rtvDescriptorHeapDesc = {};
    rtvDescriptorHeapDesc.NumDescriptors = m_backBufferCount;
    rtvDescriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
    rtvDescriptorHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
    rtvDescriptorHeapDesc.NodeMask = 0;

    ThrowIfFailed(m_d3dDevice->CreateDescriptorHeap(&rtvDescriptorHeapDesc, 
                                                    IID_PPV_ARGS(&m_rtvDescriptorHeap)));

    m_rtvDescriptorSize = m_d3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

    if (DXGI_FORMAT_UNKNOWN != m_depthBufferFormat) {
      D3D12_DESCRIPTOR_HEAP_DESC dsvDescriptorHeapDesc = {};
      dsvDescriptorHeapDesc.NumDescriptors = 1;
      dsvDescriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
      dsvDescriptorHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
      dsvDescriptorHeapDesc.NodeMask = 0;

      ThrowIfFailed(m_d3dDevice->CreateDescriptorHeap(&dsvDescriptorHeapDesc, 
                                                      IID_PPV_ARGS(&m_dsvDescriptorHeap)));
    }

    return true;
  }

  void
  D3D12GraphicApi::createRenderTargets() {
    for (uint64 i = 0; i < m_backBufferCount; ++i) {
      ThrowIfFailed(m_swapChain->GetBuffer(i, IID_PPV_ARGS(&m_renderTargets[i])));

      wchar_t name[25] = {};
      swprintf_s(name, L"Render target %u", i);
      m_renderTargets[i]->SetName(name);

      D3D12_RENDER_TARGET_VIEW_DESC rtvDesc = {};
      rtvDesc.Format = m_backBufferFormat;
      rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;

      CD3DX12_CPU_DESCRIPTOR_HANDLE rtvDescriptor(m_rtvDescriptorHeap->GetCPUDescriptorHandleForHeapStart(), 
                                                  i, 
                                                  m_rtvDescriptorSize);

      m_d3dDevice->CreateRenderTargetView(m_renderTargets[i].Get(), 
                                          &rtvDesc, 
                                          rtvDescriptor);
    }

    //Reset the index to the current back buffer.
    m_backBufferIndex = m_swapChain->GetCurrentBackBufferIndex();
  }

  bool
  D3D12GraphicApi::createDepthStencil() {
    //Determine the render target size in pixels.
    UINT backBufferWidth = max(m_outputSize.right - m_outputSize.left, 1);
    UINT backBufferHeight = max(m_outputSize.bottom - m_outputSize.top, 1);

    if (DXGI_FORMAT_UNKNOWN != m_depthBufferFormat) {
      //Allocate a 2-D surface as the depth/stencil buffer and create a depth/stencil view
      //on this surface.
      CD3DX12_HEAP_PROPERTIES depthHeapProperties(D3D12_HEAP_TYPE_DEFAULT);

      //This depth stencil view has only one texture.
      //Use a single mipmap level.
      D3D12_RESOURCE_DESC depthStencilDesc = CD3DX12_RESOURCE_DESC::Tex2D(m_depthBufferFormat,
                                                                          backBufferWidth,
                                                                          backBufferHeight,
                                                                          1, 
                                                                          1);

      depthStencilDesc.Flags |= D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

      D3D12_CLEAR_VALUE depthOptimizedClearValue = {};
      depthOptimizedClearValue.Format = m_depthBufferFormat;
      depthOptimizedClearValue.DepthStencil.Depth = 1.0f;
      depthOptimizedClearValue.DepthStencil.Stencil = 0;

      ThrowIfFailed(m_d3dDevice->CreateCommittedResource(&depthHeapProperties,
                                                         D3D12_HEAP_FLAG_NONE,
                                                         &depthStencilDesc,
                                                         D3D12_RESOURCE_STATE_DEPTH_WRITE,
                                                         &depthOptimizedClearValue,
                                                         IID_PPV_ARGS(&m_depthStencil)));
      m_depthStencil->SetName(L"Depth stencil");

      D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
      dsvDesc.Format = m_depthBufferFormat;
      dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;

      m_d3dDevice->CreateDepthStencilView(m_depthStencil.Get(),
                                          &dsvDesc, 
                                          m_dsvDescriptorHeap->GetCPUDescriptorHandleForHeapStart());
    }

    // Set the 3D rendering viewport and scissor rectangle to target the entire window.
    m_screenViewport.TopLeftX = m_screenViewport.TopLeftY = 0.f;
    m_screenViewport.Width = static_cast<float>(backBufferWidth);
    m_screenViewport.Height = static_cast<float>(backBufferHeight);
    m_screenViewport.MinDepth = D3D12_MIN_DEPTH;
    m_screenViewport.MaxDepth = D3D12_MAX_DEPTH;

    m_frameIndex = m_swapChain->GetCurrentBackBufferIndex();

    m_pRtvStart = m_rtvDescriptorHeap->GetCPUDescriptorHandleForHeapStart();

    //waitForGpu();

    return true;
  }

  void
  D3D12GraphicApi::shutdown() {
    //release();
    //release();
  }
    
  template<typename T>
  constexpr void 
  D3D12GraphicApi::release(T*& resource) {
    if (resource) {
      resource->Release();
      resource = nullptr;
    }
  }
  

  /***************************************************************************/
  /**
  * Gets.
  */
  /***************************************************************************/

  void 
  D3D12GraphicApi::getHardwareAdapter(IDXGIFactory1* pFactory1, 
                                      IDXGIAdapter1** ppAdapter1,
                                      bool requestHighPerformaAdapter) {
    *ppAdapter1 = nullptr;

    ComPtr<IDXGIAdapter1> localAdapter;
    
    ComPtr<IDXGIFactory6> localFactory;

    if (SUCCEEDED(pFactory1->QueryInterface(IID_PPV_ARGS(&localFactory)))) {
      for (uint32 adapterIndex = 0;
           SUCCEEDED(localFactory->EnumAdapterByGpuPreference(adapterIndex,
                                                              requestHighPerformaAdapter == true ?
                                                              DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE : 
                                                              DXGI_GPU_PREFERENCE_UNSPECIFIED,
                                                              IID_PPV_ARGS(&localAdapter)));
           ++adapterIndex) {
        DXGI_ADAPTER_DESC1 desc;
        localAdapter->GetDesc1(&desc);

        if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) {
          continue;
        }

        if (SUCCEEDED(D3D12CreateDevice(localAdapter.Get(), 
                                        D3D_FEATURE_LEVEL_11_0, 
                                        __uuidof(ID3D12Device), 
                                        nullptr))) {
          break;
        }
      }
    }

    if (nullptr == localAdapter.Get()) {
      for (uint32 adapterIndex = 0;
           SUCCEEDED(pFactory1->EnumAdapters1(adapterIndex, &localAdapter));
           ++adapterIndex) {
        DXGI_ADAPTER_DESC1 desc;

        localAdapter->GetDesc1(&desc);

        if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) {
          continue;
        }
        if (SUCCEEDED(D3D12CreateDevice(localAdapter.Get(), 
                                        D3D_FEATURE_LEVEL_11_0,
                                        __uuidof(ID3D12Device),
                                        nullptr))) {
          break;
        }
      }
    }

    *ppAdapter1 = localAdapter.Detach();
  }


  /***************************************************************************/
  /**
  * Sets.
  */
  /***************************************************************************/

  void
  D3D12GraphicApi::setRenderTarget(WeakSPtr<RenderTarget> pRt) {
    auto rtvHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE(m_pRtvStart, 
                                                   m_frameIndex, 
                                                   m_rtvDescriptorSize);

    m_commandList->OMSetRenderTargets(1, &rtvHandle, false, nullptr);

    auto res = CD3DX12_RESOURCE_BARRIER::Transition(m_renderTargets[m_frameIndex].Get(), 
                                                    D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);

    m_commandList->ResourceBarrier(1, &res);
  }

  void 
  D3D12GraphicApi::setViewports(const uint32 width, 
                                const uint32 heigth, 
                                const uint32 numViewports) {
    D3D12_VIEWPORT vpData;
    m_screenViewport.Width = (FLOAT)width;
    m_screenViewport.Height = (FLOAT)heigth;
    m_screenViewport.MinDepth = 0;
    m_screenViewport.MaxDepth = 1;
    m_screenViewport.TopLeftX = 0;
    m_screenViewport.TopLeftY = 0;

    m_commandList->RSSetViewports(1, &m_screenViewport);
  }

}