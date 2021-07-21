#include <gaVector2.h>

#include "gaModels.h"
#include "gaGraphicsApi.h"
#include "gaMesh.h"
#include "gaResourceManager.h"

namespace gaEngineSDK {
  /***************************************************************************/
  /*
  * Gets.
  */
  /***************************************************************************/

  SPtr<SamplerState>
  Model::getSampler() {
    return m_sampler;
  }

  Vector<SPtr<Mesh>>
  Model::getMeshesInfo() {
    return m_vMeshes;
  }

  /***************************************************************************/
  /*
  * Sets.
  */
  /***************************************************************************/

  void
  Model::setSampler(WeakSPtr<SamplerState> sampler) {
    m_sampler.reset(sampler.lock().get());
  }

  void Model::setMeshesInfo(SPtr<Mesh> meshInfo) {
    m_vMeshes.push_back(meshInfo);
  }
}