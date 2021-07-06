#pragma once

#include "gaPrerequisitesCore.h"
#include "gaGameObject.h"

namespace gaEngineSDK {
  class Actor final : public GameObject
  {
    public:
      /***********************************************************************/
      /**
      * Constructor and destructor.
      */
      /***********************************************************************/
      Actor() = default;

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
      update(const float& deltaTime) override;

      /*
      * @brief .
      */
      void
      render() override;

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
      SPtr<Component>
      setComponent(TYPE_COMPONENTS::E typeComponent, 
                   SPtr<Component> component = nullptr) override;

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
      String m_actorName = "Default actor name";

      /*
      * @brief .
      */
      Map<TYPE_COMPONENTS::E, SPtr<Component>> m_mapComponents;
  };
}