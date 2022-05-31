//*********************************************************
#pragma once

#include "gaD3D12GraphicApi.h"

namespace gaEngineSDK {
  class Win32Application
  {
  public:
    /*************************************************************************/
    /**
    * Methods.
    */
    /*************************************************************************/
    static int 
    run(D3D12GraphicApi* pSample, HINSTANCE hInstance, int nCmdShow);

    static void 
    toggleFullscreenWindow(IDXGISwapChain* pOutput = nullptr);

    static void 
    setWindowZorderToTopMost(bool setToTopMost);

    static HWND 
    getHwnd() { return m_hwnd; }

    static bool 
    isFullscreen() { return m_fullscreenMode; }

  protected:
    static LRESULT CALLBACK 
    windowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

  private:
    /*************************************************************************/
    /**
    * Members.
    */
    /*************************************************************************/

    static HWND m_hwnd;

    static bool m_fullscreenMode;

    static const UINT m_windowStyle = WS_OVERLAPPEDWINDOW;

    static RECT m_windowRect;
  };
}