#pragma once

#include "gaPrerequisitesCore.h"
#include "gaModule.h"
#include "gaActor.h"
#include "gaSceneNode.h"

namespace gaEngineSDK {
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
      init(String nameRoot);

      /*
      * @brief
      */
      void
      update(const float& deltaTime);

      /*
      * @brief
      */
      SPtr<GameObject>
      createNewGameObj();

    private:
      /***********************************************************************/
      /**
      * Members.
      */
      /***********************************************************************/

      /*
      * @brief sceneNode?
      */
      SPtr<Actor> m_root = nullptr;

      /*
      * @brief
      */
      SPtr<SceneNode> m_rootNode = nullptr;

      /*
      * @brief
      */
      SPtr<Actor> m_selectedActor = nullptr;

      /*
      * @brief
      */
      Vector<SPtr<Actor>> m_vActors;
  };
}