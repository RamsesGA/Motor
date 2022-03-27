#include <exception>

#include "gaD3D12GraphicApi.h"

namespace gaEngineSDK {
  constexpr D3D_FEATURE_LEVEL g_minimumFeatureLvl = D3D_FEATURE_LEVEL_11_0;

  /***************************************************************************/
  /**
  * Inheritance methods.
  */
  /***************************************************************************/

  bool 
  D3D12GraphicApi::initDevice(sf::WindowHandle hWnd) {
    //Hardware result.
    HRESULT hr;

#if defined(DEBUG) || defined(_DEBUG)
    ComPtr<ID3D12Debug> debugController;
    //ThrowIfFailed(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)));
    debugController->EnableDebugLayer();
#endif


    hr = D3D12CreateDevice(m_pAdapter.Get(), 
                           m_d3dMinFeatureLevel, 
                           IID_PPV_ARGS(&m_d3dDevice));
    if (FAILED(hr)) {
      cout << "\nError, failed D3D12CreateDevice\n";
      return false;
    }


    /*
    //Determine what is the maximum feature level that is supporter.
    //Create D3D12 device (this is a virtual adapter).
    if (m_pMainDevice) {
      shutdown();
      return false;
    }

    RECT rc;

    GetClientRect(hWnd, &rc);

    m_width = rc.right - rc.left;
    m_height = rc.bottom - rc.top;

    uint32 dxgiFactoryFlags = 0;
    HRESULT hr;

#if defined (_DEBUG)
    {
      ComPtr<ID3D12Debug3> localDebugUI;
      hr = D3D12GetDebugInterface(IID_PPV_ARGS(&localDebugUI));

      if (FAILED(hr)) {
        localDebugUI->EnableDebugLayer();
      }
    }

    dxgiFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
#endif

    hr = CreateDXGIFactory2(dxgiFactoryFlags, IID_PPV_ARGS(&m_pDxgiFactory));

    if (FAILED(hr)) {
      cout << "\nError, failed to create DXGI Factory 2\n";
      shutdown();
      return false;
    }

    //Determine which adapter (graphics card) to use, if any.
    ComPtr<IDXGIAdapter4> mainAdapter;
    mainAdapter.Attach(determineMainAdapter());

    if (!(mainAdapter)) {
      cout << "\nError, main adapter failed\n";
      shutdown();
      return false;
    }

    D3D_FEATURE_LEVEL localMaxFeatureLvl = getMaxFeatureLvl(mainAdapter.Get());

    assert(localMaxFeatureLvl >= g_minimumFeatureLvl);

    if (localMaxFeatureLvl < g_minimumFeatureLvl) {
      cout << "\nError trying to create max feature level\n";
      return false;
    }

    hr = D3D12CreateDevice(mainAdapter.Get(), 
                           localMaxFeatureLvl, 
                           IID_PPV_ARGS(&m_pMainDevice));

    if (FAILED(hr)) {
      cout << "\nError, trying to create D3D12 Create Device\n";
      shutdown();
      return false;
    }

    m_pMainDevice->SetName(L"Main Device");

    //Back Buffer
    //auto* backBuffer = new TexturesDX();
    //
    //hr = m_pSwapChain->GetBuffer(0, 
    //                             __uuidof(ID3D11Texture2D), 
    //                             (LPVOID*)&backBuffer->m_pTexture);
    //
    ////We check that everything goes well, if we do not send an error.
    //if (FAILED(hr)) {
    //  _com_error err(hr);
    //  LPCTSTR errMsg = err.ErrorMessage();
    //  delete backBuffer;
    //  return false;
    //}
    //
    //backBuffer->m_vRenderTargetView.resize(1);
    //hr = m_pd3dDevice->CreateRenderTargetView(backBuffer->m_pTexture,
    //                                          nullptr,
    //                                          &backBuffer->m_vRenderTargetView[0]);
    //
    //if (FAILED(hr)) {
    //  _com_error err(hr);
    //  LPCTSTR errMsg = err.ErrorMessage();
    //  delete backBuffer;
    //  return false;
    //}
    //
    //m_pBackBuffer = backBuffer;

    return true;
    */
  }

  /***************************************************************************/
  /**
  * Methods.
  */
  /***************************************************************************/

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
}