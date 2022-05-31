/*
#pragma once

#include <d3d12.h>
#include <dxgiformat.h>
#include <d3dcommon.h>

#include "gaD3D12Prerequisites.h"


namespace gaEngineSDK {

  using std::cout;

  interface 
  IDeviceNotify {
    virtual void onDeviceLost() = 0;
    virtual void onDeviceRestored() = 0;
  };

  class D3D12DeviceResource
  {
  public:
    

    D3D12DeviceResource() = default;

    D3D12DeviceResource(DXGI_FORMAT backBufferFormat = DXGI_FORMAT_B8G8R8A8_UNORM,
                        DXGI_FORMAT depthBufferFormat = DXGI_FORMAT_D32_FLOAT,
                        uint32 backBufferCount = 2,
                        D3D_FEATURE_LEVEL minFeatureLevel = D3D_FEATURE_LEVEL_11_0,
                        uint32 flags = 0,
                        uint32 adapterIDoverride = UINT_MAX);
    
    ~D3D12DeviceResource() = default;

    

    void 
    registerDeviceNotify(IDeviceNotify* deviceNotify) {
      m_deviceNotify = deviceNotify;

      __if_exists(DXGIDeclareAdapterRemovalSupport) {
        if (deviceNotify) {
          if (FAILED(DXGIDeclareAdapterRemovalSupport())) {
            OutputDebugString("\nWarning: application failed to declare adapter removal support.\n");
          }
        }
      }
    }

    void 
    setWindow(HWND window, int32 width, int32 height);

    
    
    const static size_t MAX_BACK_BUFFER_COUNT = 3;
    static const uint32 m_c_allowTearing = 0x1;
    static const uint32 m_c_requireTearingSupport = 0x2;

    uint32 m_backBufferIndex;
    uint32 m_rtvDescriptorSize;
    uint32 m_backBufferCount;
    uint32 m_options;
    uint32 m_adapterIDoverride;
    uint32 m_adapterID;

    uint64 m_fenceValues[MAX_BACK_BUFFER_COUNT];

    D3D12_VIEWPORT m_screenViewport;
    D3D12_RECT m_scissorRect;

    DXGI_FORMAT m_backBufferFormat;
    DXGI_FORMAT m_depthBufferFormat;

    D3D_FEATURE_LEVEL m_d3dMinFeatureLevel;
    D3D_FEATURE_LEVEL m_d3dFeatureLevel;

    HWND m_window;

    IDeviceNotify* m_deviceNotify;
  };
}
*/

/*****************************************************************************/
/**
* Controls all the DirectX device resources.
*/
/*****************************************************************************/

/*************************************************************************/
    /**
    * Constructor and destructor.
    */
    /*************************************************************************/

/*************************************************************************/
    /**
    * Methods.
    */
    /*************************************************************************/

/*************************************************************************/
    /**
    * Members.
    */
    /*************************************************************************/