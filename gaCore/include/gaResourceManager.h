#pragma once

#include <gaModule.h>

#include "gaPrerequisitesCore.h"
#include "gaResource.h"
#include "gaModels.h"

namespace gaEngineSDK {
  using std::make_shared;
  using std::make_pair;
  using std::dynamic_pointer_cast;

  class GA_CORE_EXPORT ResourceManager : public Module<ResourceManager>
  {
   public:
    /*************************************************************************/
    /**
    * Constructor and destructor.
    */
    /*************************************************************************/
    ResourceManager() = default;

    ~ResourceManager() = default;

    /*************************************************************************/
    /**
    * Methods.
    */
    /*************************************************************************/

    /*
    * @brief .
    */
    template<typename T>
    SPtr<T>
    load(String file) {
      if (isAlreadyLoaded(file)) {
        return dynamic_pointer_cast<T>(m_mLoadedResources[file]);
      }
      
      m_mLoadedResources.insert(make_pair(file, make_shared<T>()));
      auto& resourceObj = m_mLoadedResources[file];
      resourceObj->loadFromFile(file);
      
      return dynamic_pointer_cast<T>(resourceObj);
    }

    /*
    * @brief .
    */
    template<typename T>
    SPtr<T>
    load(String file, TYPE_TEXTURES::E typeTexture) {
      if (isAlreadyLoaded(file)) {
        return dynamic_pointer_cast<T>(m_mLoadedResources[file]);
      }
      
      m_mLoadedResources.insert(make_pair(file, make_shared<T>()));
      auto& resourceObj = m_mLoadedResources[file];
      resourceObj->loadFromFile(file, typeTexture);
      
      return dynamic_pointer_cast<T>(resourceObj);
    }

    bool
    isAlreadyLoaded(String file);

   private:
    /*************************************************************************/
    /**
    * Members.
    */
    /*************************************************************************/

    /*
    * @brief .
    */
    Map<String, SPtr<Resource>> m_mLoadedResources;
  };
}