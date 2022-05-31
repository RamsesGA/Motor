#include "gaD3D12Renderer.h"

namespace gaEngineSDK {
  void 
  D3D12Renderer::init(uint32 width, uint32 height) {
    auto myGraphicsApi = g_graphicApi().instancePtr();

    m_width = width;
    m_height = height;

    myGraphicsApi->setViewports(m_width, m_height);
  }

  void
  D3D12Renderer::update(const float& deltaTime) {
    auto myGraphicsApi = g_graphicApi().instancePtr();

    m_totalTime += deltaTime;

    m_colors[0] = std::sin(m_totalTime);
    m_colors[1] = std::cos(-m_totalTime);
  }

  void
  D3D12Renderer::render() {
    auto myGraphicsApi = g_graphicApi().instancePtr();

    myGraphicsApi->setViewports(m_width, m_height);

    myGraphicsApi->clear(m_pRenderTarget, m_colors[0], m_colors[1], 1, 1);
    
    myGraphicsApi->setRenderTarget(m_pRenderTarget);
  }
}