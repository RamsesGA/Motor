#pragma once

#include "gaPrerequisitesCore.h"
#include "gaComponent.h"

namespace gaEngineSDK {
  class GA_CORE_EXPORT GameObject
  {
   public:
    /*************************************************************************/
    /**
    * Constructor and destructor.
    */
    /*************************************************************************/
    GameObject() = default;

    ~GameObject() = default;

    /*************************************************************************/
    /**
    * Methods.
    */
    /*************************************************************************/

    /*
    * @brief Removing the component from an actor.
    */
    virtual void
    removeComponent() { };

    /*
    * @brief Actor information update.
    */
    virtual void
    actorRender() { };

    /*************************************************************************/
    /**
    * Gets.
    */
    /*************************************************************************/

    /*
    * @brief Notify if the actor is selected.
    * @return Bool member to indicate true or false.
    */
    virtual bool
    getIsSelected() { return false; };

    /*
    * @brief Get the name of the current actor.
    * @return String with the member's information.
    */
    virtual String
    getActorName() { return "Default"; };

    /*
    * @brief .
    */
    virtual SPtr<Component>
    getComponent() { return nullptr; };

    /*************************************************************************/
    /**
    * Sets.
    */
    /*************************************************************************/

    /*
    * @brief Save the corresponding information in the member.
    * @param isSelect, bool to indicate true or false.
    */
    virtual void
    setIsSelected(bool isSelect) { };

    /*
    * @brief Save the corresponding information in the member.
    * @param name of the actor.
    */
    virtual void
    setActorName(String name) { };

    /*
    * @brief Save the corresponding information in the member.
    * @param compoInfo, pointer with the information of a component.
    */
    virtual void
    setComponent(WeakSPtr<Component> compoInfo) { };

    
  };
}