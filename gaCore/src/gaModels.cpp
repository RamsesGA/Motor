#include <gaVector2.h>

#include "gaModels.h"
#include "gaGraphicsApi.h"
#include "gaMesh.h"
#include "gaResourceManager.h"

namespace gaEngineSDK {

  void
  Model::setSamplers(SamplerState* sampler) {
    m_pSamplers.push_back(sampler);
  }

  Vector<SamplerState*>
  Model::getSamplerInfo() {
    return m_pSamplers;
  }
}