#pragma once

#include "gaPrerequisitesCore.h"

namespace gaEngineSDK {

  namespace TYPE_COMPONENTS {
    enum E {
      kRenderModel = 0,
      kTransform,
      kCamera,
      kTexture,
    };
  };

  class Actor;

  class GA_CORE_EXPORT Component
  {
    public:
      /***********************************************************************/
      /**
      * Constructor and destructor.
      */
      /***********************************************************************/

      Component() = default;
      
      ~Component() = default;

      /***********************************************************************/
      /**
      * Methods.
      */
      /***********************************************************************/

      /*
      * @brief .
      */
      virtual void
      update(const float& deltatime) { };

      /***********************************************************************/
      /**
      * Gets.
      */
      /***********************************************************************/

      /*
      * @brief .
      */
      TYPE_COMPONENTS::E
      getTypeComponents() const;

      /***********************************************************************/
      /**
      * Sets.
      */
      /***********************************************************************/

      /*
      * @brief .
      */
      void
      setActor(Actor* newObj);

    protected:
      /***********************************************************************/
      /**
      * Members.
      */
      /***********************************************************************/

      /*
      * @brief .
      */
      TYPE_COMPONENTS::E m_typeComponents;

      /*
      * @brief .
      */
      Actor* m_newActorObj = nullptr;
  };
}