#pragma once

#include <gaBasePhysics.h>

//Nvidia


#include "gaPrerequisitesPhysics.h"

namespace gaEngineSDK {
  class Physics final : public BasePhysics
  {
  public:
    /*************************************************************************/
    /**
    * Constructor and destructor.
    */
    /*************************************************************************/

    Physics() = default;

    ~Physics() = default;

    /*************************************************************************/
    /**
    * Basic methods.
    */
    /*************************************************************************/

    /*
    * @brief .
    */
    void
    init(uint32 width, uint32 height, void* hwndHandle);

    /*
    * @brief .
    */
    void
    update(const float& deltaTime);
  };

  /***************************************************************************/
  /**
  * Export.
  */
  /***************************************************************************/
  extern "C" GA_PHYSICS_EXPORT BasePhysics *
  newPhysics() {
    Physics* pPhysics = new Physics();
    return pPhysics;
  }
}