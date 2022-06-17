#pragma once
#pragma warning( disable: 4251 )

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
    update(const float& deltaTime) {
      GA_UNREFERENCED_PARAMETER(deltaTime);
    };

    /*
    * @brief Component information rendering.
    */
    virtual void
    render() { };

    WeakSPtr<Actor> m_pMyActor;
  };
}