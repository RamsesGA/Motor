#include "gaGraphicsApiOGL.h"

namespace gaEngineSDK {
  extern "C" _declspec(dllexport) GraphicsApi * createGraphicsApiOGL() {
    return new GraphicsApiOGL();
  }
}