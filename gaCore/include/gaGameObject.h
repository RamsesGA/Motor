#pragma once

#include "gaPrerequisitesCore.h"
#include "gaComponent.h"

namespace gaEngineSDK {

  class GA_CORE_EXPORT GameObject 
  {
    public:
      /***********************************************************************/
      /**
      * Constructor and destructor.
      */
      /***********************************************************************/
      GameObject() = default;
      
      ~GameObject() = default;
    
      /***********************************************************************/
      /**
      * Methods.
      */
      /***********************************************************************/

      /*
      * @brief .
      */
      virtual void
      removeComponent(TYPE_COMPONENTS::E typeComponent) { };

      /***********************************************************************/
      /**
      * Gets.
      */
      /***********************************************************************/

      /*
      * @brief .
      */
      virtual bool
      getIsSelected() { return false; };

      /*
      * @brief .
      */
      virtual String 
      getActorName() { return "Default"; };

      /*
      * @brief .
      */
      virtual SPtr<Component>
      getComponent(TYPE_COMPONENTS::E typeComponent) { return nullptr; };

      /***********************************************************************/
      /**
      * Sets.
      */
      /***********************************************************************/

      /*
      * @brief .
      */
      virtual void
      setIsSelected(bool isSelect) { };

      /*
      * @brief .
      */
      virtual void
      setActorName(String name) { };

      /*
      * @brief .
      */
      virtual void
      setComponent(TYPE_COMPONENTS::E typeComponent, WeakSPtr<Component> compoInfo) { };
  };
}