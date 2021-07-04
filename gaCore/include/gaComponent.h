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

  class GameObject;

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
      update(const float& deltatime);

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
      setGameObject(GameObject* newObj);

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
      GameObject* m_newObject = nullptr;
  };
}