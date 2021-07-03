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

  uint32 
  Model::getNumAnims() {
    return m_numAnimations;
  }

  Vector<String> 
  Model::getAnimList() {
    return m_animationsList;
  }

  SPtr<SamplerState>
  Model::getSampler() {
    return m_sampler;
  }

  SPtr<AnimationData>
  Model::getCurrentAnim() {
    return m_currentAnimation;
  }

  SPtr<ResourceManager>
  Model::getResourceMgrInfo() {
    return m_resourceManager;
  }

  Vector<SPtr<Mesh>>
  Model::getMeshesInfo() {
    return m_vMeshes;
  }

  Vector<SPtr<AnimationData>>
  Model::getAnimData() {
    return m_vAnimationData;
  }

  /***************************************************************************/
  /*
  * Sets.
  */
  /***************************************************************************/

  void
  Model::setNumAnims(uint32 numAnims) {
    m_numAnimations = numAnims;
  }

  void
  Model::setAnimList(String animName) {
    m_animationsList.push_back(animName);
  }

  void
  Model::setSampler(WeakSPtr<SamplerState> sampler) {
    m_sampler.reset(sampler.lock().get());
  }

  void 
  Model::setCurrentAnim(WeakSPtr<AnimationData> currentAnim) {
    m_currentAnimation.reset(currentAnim.lock().get());
  }

  void 
  Model::setResourceMgrInfo(WeakSPtr<ResourceManager> resourceMgr) {
    m_resourceManager.reset(resourceMgr.lock().get());
  }

  void Model::setMeshesInfo(SPtr<Mesh> meshInfo) {
    m_vMeshes.push_back(meshInfo);
  }

  void
  Model::setAnimData(SPtr<AnimationData> animData) {
    m_vAnimationData.push_back(animData);
  }
}