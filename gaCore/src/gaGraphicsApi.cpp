#include "gaGraphicsApi.h"

namespace gaEngineSDK {
  GraphicsApi& 
  g_graphicApi() {
    return GraphicsApi::instance();
  }
}