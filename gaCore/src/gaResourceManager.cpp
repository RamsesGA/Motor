#include <assimp/scene.h>
#include <assimp/cimport.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

#include "gaResourceManager.h"
#include "gaGraphicsApi.h"

namespace gaEngineSDK {

  bool 
  ResourceManager::isAlreadyLoaded(String file) {
    return (m_mLoadedResources.find(file) != m_mLoadedResources.end());
  }
}