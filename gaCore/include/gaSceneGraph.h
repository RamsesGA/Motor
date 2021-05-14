#pragma once

#include <gaModule.h>

#include "gaGameObject.h"
#include "gaPrerequisitesCore.h"

namespace gaEngineSDK 
{
  class GA_CORE_EXPORT SceneGraph : public Module<SceneGraph>
  {
    public:
      /***********************************************************************/
      /**
      * Constructor and destructor.
      */
      /***********************************************************************/
      SceneGraph() = default;
      
      ~SceneGraph() = default;

      /***********************************************************************/
      /**
      * Methods.
      */
      /***********************************************************************/
      
      /*
      * @brief
      */
      void
      onInit();

      /*
      * @brief
      */
      void
      onUpdate(const float& deltaTime);

      /*
      * @brief
      */
      void
      onDelete();

      /*
      * @brief
      */
      SPtr<GameObject>
      createNewGameObj();

    private:
      /**********************************************************************/
      /**
      * Members.
      */
      /**********************************************************************/

      /*
      * @brief
      */
      Vector <SPtr<GameObject>> m_vGameObjects;

      /*
      * @brief
      */
      SPtr<GameObject> m_selectedObj = nullptr;

      /*
      * @brief
      */
      SPtr<GameObject> m_root = nullptr;
  };
}