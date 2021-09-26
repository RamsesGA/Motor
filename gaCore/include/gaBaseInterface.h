#pragma once

#include <gaModule.h>

#include "gaPrerequisitesCore.h"

namespace gaEngineSDK {
  class GA_CORE_EXPORT BaseInterface : public Module<BaseInterface>
  {
   public:
    /*************************************************************************/
    /**
    * Constructor and destructor.
    */
    /*************************************************************************/

    BaseInterface() = default;

    ~BaseInterface() = default;

    /*
    * @brief .
    */
    void
    setObject(BaseInterface* api) {
      BaseInterface::_instance() = api;
    }

    /*
    * @brief .
    */
    virtual void
    init(uint32 width, uint32 height, void* hwndHandle) {};

    /*
    * @brief .
    */
    virtual void
    update(const float& deltaTime) {};

    /*
    * @brief .
    */
    virtual void
    render() {};

    bool m_touchingImGui = false;
  };

  /***************************************************************************/
  /**
  * INSTANCE.
  */
  /***************************************************************************/

  GA_CORE_EXPORT BaseInterface&
  g_baseInterface();
}