#pragma once

#include <gaBaseRenderer.h>
#include <gaRenderTarget.h>
#include <gaGraphicsApi.h>
#include <SFML/Window.hpp>

#include "gaPrerequisitesD3D12Render.h"

namespace gaEngineSDK {
  using sf::WindowHandle;

  class D3D12Renderer final : public BaseRenderer
  {
   public:
    /*************************************************************************/
    /**
    * Constructor and destructor.
    */
    /*************************************************************************/

    D3D12Renderer() = default;

    ~D3D12Renderer() = default;

    /*************************************************************************/
    /**
    * Inheritance.
    */
    /*************************************************************************/

    /*
    * @brief .
    */
    void
    init(uint32 width, uint32 height) override;

    /*
    * @brief .
    */
    void
    update(const float& deltaTime) override;

    /*
    * @brief .
    */
    void
    render() override;

   protected:
    /*************************************************************************/
    /**
    * Members.
    */
    /*************************************************************************/

    /*
    * @param Members with screen size.
    */
    uint32 m_width = 0;
    uint32 m_height = 0;

    uint32 m_totalTime = 0;

    float m_colors[4] = { 0,0,1,1 };

    WindowHandle m_hWnd;

    SPtr<RenderTarget> m_pRenderTarget;
  };

  /***************************************************************************/
  /**
  * Export.
  */
  /***************************************************************************/
  extern "C" GA_D3D12_REND_EXPORT BaseRenderer *
  createD3D12Renderer() {
    D3D12Renderer* pDefRender = new D3D12Renderer();
    return pDefRender;
  }
}