#include "gaBaseRenderer.h"

namespace gaEngineSDK {
  /***************************************************************************/
  /**
  * INSTANCE.
  */
  /***************************************************************************/
  BaseRenderer&
  g_baseRenderer() {
    return BaseRenderer::instance();
  }
}