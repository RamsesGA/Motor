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
    init(uint32 width, uint32 height, void* hwndHandle) {
      GA_UNREFERENCED_PARAMETER(width);
      GA_UNREFERENCED_PARAMETER(height);
      GA_UNREFERENCED_PARAMETER(hwndHandle);
    };

    /*
    * @brief .
    */
    virtual void
    update(const float& deltaTime) {
      GA_UNREFERENCED_PARAMETER(deltaTime);
    };
  };

  /***************************************************************************/
  /**
  * INSTANCE.
  */
  /***************************************************************************/

  GA_CORE_EXPORT BasePhysics&
  g_basePhysics();
}