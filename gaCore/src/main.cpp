#include <iostream>
#include <Windows.h>

#include "gaGraphicsApi.h"
#include "gaTextures.h"

namespace gaEngineSDK {
  int
  main() {
    HINSTANCE hInstance = LoadLibraryExA("gaDirectX.dll", nullptr,
                                         LOAD_WITH_ALTERED_SEARCH_PATH);

    if (!(hInstance)) {
      return -1;
    }

    using fnProt = GraphicsApi* (*)();

    fnProt graphicsApiFunc = reinterpret_cast<fnProt>
                             (GetProcAddress(hInstance, "createGraphicsApiDX"));

    if (!(graphicsApiFunc)) {
      return -1;
    }

    GraphicsApi* graphicsApiDX = graphicsApiFunc();

    FreeLibrary(hInstance);

    return 0;
  }
}


