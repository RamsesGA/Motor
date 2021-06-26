#pragma once

#include "gaPrerequisitesCore.h"

namespace gaEngineSDK {

  namespace TYPE_COMPONENTS {
    enum E {
      TRANSFORM = 0,
      RENDERMODEL,
      TEXTURE,
      CAMERA,
      KNUMCOMPONENTS
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
      TYPE_COMPONENTS::E
      getTypeComponents() const;

      /*
      * @brief .
      */
      virtual void
      update(const float& deltatime);

      /*
      * @brief .
      */
      void
      setGameObject(GameObject* newObj);

    private:
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