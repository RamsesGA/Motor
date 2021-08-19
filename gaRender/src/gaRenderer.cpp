#include "gaRenderer.h"

namespace gaEngineSDK {
  Renderer&
   g_renderer() {
    return Renderer::instance();
  }
}