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
      removeComponent() override;

      /*
      * @brief .
      */
      void
      actorUpdate(const float& deltaTime);

      /*
      * @brief .
      */
      void
      actorRender() override;

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
      setComponent(WeakSPtr<Component> compoInfo) override;

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
      Vector<SPtr<Component>> m_vComponents;
  };
}