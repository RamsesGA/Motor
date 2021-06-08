#include <gaVector2.h>

#include "gaModels.h"
#include "gaGraphicsApi.h"
#include "gaMesh.h"
#include "gaResourceManager.h"

namespace gaEngineSDK {


  void
  Model::draw(ResourceManager& resource) {
    auto myGraphicApi = g_graphicApi().instancePtr();

    if (nullptr == myGraphicApi) {
      throw new std::exception("Error, parametro nulo en Draw del modelo");
    }

    for (uint32 i = 0; i < resource.getMeshes().size(); i++) {
      resource.getMeshes()[i]->draw(resource.getSamplerInfo());
    }
  }

  void
  Model::setSamplers(SamplerState* sampler) {
    m_pSamplers.push_back(sampler);
  }

  Vector<SamplerState*>
  Model::getSamplerInfo() {
    return m_pSamplers;
  }
}