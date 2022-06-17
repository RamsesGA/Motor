#pragma once

#include "gaPrerequisitesCore.h"
#include "gaModule.h"
#include "gaSceneNode.h"

namespace gaEngineSDK {
  class Actor;
  class ResourceManager;
  class Models;

  class GA_CORE_EXPORT SceneGraph : public Module<SceneGraph>
  {
   public:
    /*************************************************************************/
    /**
    * Constructor and destructor.
    */
    /*************************************************************************/

    SceneGraph();

    ~SceneGraph() = default;

    /*************************************************************************/
    /**
    * Methods.
    */
    /*************************************************************************/

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
    SPtr<SceneNode>
    createNewActor(WeakSPtr<Actor> actor, WeakSPtr<SceneNode> parent);

    void
    clearSceneGraph();

    /*************************************************************************/
    /**
    * Members.
    */
    /*************************************************************************/

    SPtr<SceneNode> m_nodeSelected;

    /*
    * @brief .
    */
    SPtr<SceneNode> m_root;

   private:

    /*
    * @brief .
    */
    SPtr<Models> m_accessModel;
  };

  /***************************************************************************/
  /**
  * Export.
  */
  /***************************************************************************/

  GA_CORE_EXPORT SceneGraph&
  g_sceneGraph();
}