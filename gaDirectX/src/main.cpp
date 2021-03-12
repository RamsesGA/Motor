#include "gaGraphicsApiDX.h"

namespace gaEngineSDK {
  extern "C" _declspec(dllexport) GraphicsApi * createGraphicsApiDX() {
    return new GraphicsApiDX();
  }
}