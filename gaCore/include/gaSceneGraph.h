#pragma once

#include "gaPrerequisitesCore.h"
#include "gaModule.h"
#include "gaSceneNode.h"

namespace gaEngineSDK {

  class Actor;

  class GA_CORE_EXPORT SceneGraph : public Module<SceneGraph>
  {
    public:
      /***********************************************************************/
      /**
      * Constructor and destructor.
      */
      /***********************************************************************/
      
      SceneGraph();
      
      ~SceneGraph() = default;

      /***********************************************************************/
      /**
      * Methods.
      */
      /***********************************************************************/

      /*
      * @brief .
      */
      void
      update(const float& deltaTime);

      /*
      * @brief .
      */
      void
      render();

      /*
      * @brief .
      */
      void
      createNewActor(WeakSPtr<Actor> actor, WeakSPtr<SceneNode> parent);

    private:
      /***********************************************************************/
      /**
      * Members.
      */
      /***********************************************************************/

      /*
      * @brief .
      */
      SPtr<SceneNode> m_root = nullptr;
  };

  /***************************************************************************/
  /**
  * Export.
  */
  /***************************************************************************/

  GA_CORE_EXPORT SceneGraph&
  g_sceneGraph();
}