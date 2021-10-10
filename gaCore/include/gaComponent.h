#pragma once

#include "gaPrerequisitesCore.h"

namespace gaEngineSDK {
  class Actor;

  class GA_CORE_EXPORT Component
  {
   public:
    /*************************************************************************/
    /**
    * Constructor and destructor.
    */
    /*************************************************************************/

    Component() = default;

    ~Component() = default;

    /*************************************************************************/
    /**
    * Methods.
    */
    /*************************************************************************/

    /*
    * @brief Component information update.
    * @param deltaTime, time elapsed since the last frame.
    */
    virtual void
    update(const float& deltaTime) { };

    /*
    * @brief Component information rendering.
    */
    virtual void
    render() { };

    SPtr<Actor> m_pMyActor;
  };
}