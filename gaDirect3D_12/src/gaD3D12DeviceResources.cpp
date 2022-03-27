#include "gaD3D12DeviceResources.h"

namespace gaEngineSDK {
  D3D12DeviceResource::D3D12DeviceResource(DXGI_FORMAT backBufferFormat,
                                           DXGI_FORMAT depthBufferFormat,
                                           uint32 backBufferCount,
                                           D3D_FEATURE_LEVEL minFeatureLevel,
                                           uint32 flags,
                                           uint32 adapterIDoverride) {
    m_backBufferIndex = 0;
    m_rtvDescriptorSize = 0;
    m_backBufferFormat = backBufferFormat;
    m_depthBufferFormat = depthBufferFormat;
    m_backBufferCount = backBufferCount;
    m_d3dMinFeatureLevel = minFeatureLevel;
    m_window = nullptr;
    m_d3dFeatureLevel = D3D_FEATURE_LEVEL_11_0;
    m_options = flags;
    m_adapterIDoverride = adapterIDoverride;
    m_adapterID = UINT_MAX;
    
    if (backBufferCount > MAX_BACK_BUFFER_COUNT) {
      cout<<"\nbackBufferCount too large\n";
    }
    if (minFeatureLevel < D3D_FEATURE_LEVEL_11_0) {
      cout << "\nminFeatureLevel too low\n";
    }
    if (m_options & m_c_requireTearingSupport) {
      m_options |= m_c_allowTearing;
    }
  }

  void D3D12DeviceResource::setWindow(HWND window, int32 width, int32 height)
  {

  }

}