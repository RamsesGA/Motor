#pragma once

#include <gaModule.h>

#include "gaPrerequisitesCore.h"

namespace gaEngineSDK {
  class GA_CORE_EXPORT BasePhysics : public Module<BasePhysics>
  {
   public:
    /*************************************************************************/
    /**
    * Constructor and destructor.
    */
    /*************************************************************************/

    BasePhysics() = default;

    ~BasePhysics() = default;

    /*
    * @brief .
    */
    void
    setObject(BasePhysics* api) {
      BasePhysics::_instance() = api;
    }

    /*************************************************************************/
    /**
    * Basic methods.
    */
    /*************************************************************************/

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
  };

  /***************************************************************************/
  /**
  * INSTANCE.
  */
  /***************************************************************************/

  GA_CORE_EXPORT BasePhysics&
  g_basePhysics();
}