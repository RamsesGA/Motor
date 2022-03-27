#pragma once

#include "gaD3D12Prerequisites.h"

using namespace gaEngineSDK;

struct Viewport {
  float left;
  float top;
  float right;
  float bottom;
};

struct RayGenConstantBuffer {
  Viewport viewport;
  Viewport stencil;
};