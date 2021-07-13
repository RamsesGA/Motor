#pragma once

#include <gaTransform.h>

#include "gaPrerequisitesCore.h"
#include "gaGameObject.h"
#include "gaComponent.h"

namespace gaEngineSDK {
  class GA_CORE_EXPORT Actor final : public GameObject
  {
    public:
      /***********************************************************************/
      /**
      * Constructor and destructor.
      */
      /***********************************************************************/
      
      Actor(String actorName);

      ~Actor() = default;

      /***********************************************************************/
      /**
      * Methods.
      */
      /***********************************************************************/

      /*
      * @brief .
      */
      void
      removeComponent(TYPE_COMPONENTS::E typeComponent) override;

      /***********************************************************************/
      /**
      * Gets.
      */
      /***********************************************************************/

      /*
      * @brief .
      */
      bool
      getIsSelected() override;

      /*
      * @brief .
      */
      String 
      getActorName() override;

      /*
      * @brief .
      */
      SPtr<Component>
      getComponent(TYPE_COMPONENTS::E typeComponent) override;

      /***********************************************************************/
      /**
      * Sets.
      */
      /***********************************************************************/

      /*
      * @brief .
      */
      void
      setIsSelected(bool isSelect) override;

      /*
      * @brief .
      */
      void
      setActorName(String name) override;

      /*
      * @brief .
      */
      void
      setComponent(TYPE_COMPONENTS::E typeComponent, WeakSPtr<Component> compoInfo) override;

      protected:
      /***********************************************************************/
      /**
      * Members.
      */
      /***********************************************************************/

      /*
      * @brief .
      */
      bool m_isSelected = false;

      /*
      * @brief .
      */
      String m_actorName;

      /*
      * @brief .
      */
      SPtr<Transform> m_actorTransform;

      /*
      * @brief .
      */
      SPtr<Component> m_component;

      /*
      * @brief .
      */
      Vector<TYPE_COMPONENTS::E> m_vComponents;
      
  };
}