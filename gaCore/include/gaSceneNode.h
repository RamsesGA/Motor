#pragma once

#include "gaPrerequisitesCore.h"
#include "gaActor.h"

namespace gaEngineSDK {
  class GA_CORE_EXPORT SceneNode
  {
   public:
    /*************************************************************************/
    /**
    * Constructor and destructor.
    */
    /*************************************************************************/
    SceneNode() = default;

    SceneNode(WeakSPtr<Actor> actor, WeakSPtr<SceneNode> parent);

    ~SceneNode() = default;

    /*************************************************************************/
    /**
    * Methods.
    */
    /*************************************************************************/

    /*
    * @brief .
    */
    void
    detectSelectedNode();

    /*
    * @brief .
    */
    SPtr<SceneNode>
    createNewActorNode(WeakSPtr<Actor> actor, WeakSPtr<SceneNode> parent);

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

    void
    clearSceneNodes();

    /*************************************************************************/
    /**
    * Sets.
    */
    /*************************************************************************/

    /*
    * @brief .
    */
    void
    setActorNode(WeakSPtr<Actor> actor);

    /*
    * @brief .
    */
    void
    setParentNode(WeakSPtr<SceneNode> parent);

    /*
    * @brief .
    */
    void
    setChildNode(WeakSPtr<SceneNode> child);

    /*************************************************************************/
    /**
    * Gets.
    */
    /*************************************************************************/

    /*
    * @brief .
    */
    SPtr<Actor>&
    getActorNode();

    /*
    * @brief .
    */
    WeakSPtr<SceneNode>
    getParentNode();

    /*
    * @brief .
    */
    Vector<SPtr<SceneNode>>&
    getChildNodes();

    /*
    * @brief Bool to indicate if the actor is selected.
    */
    bool m_isSelected = false;

    /*
    * @brief Save the sdf path of Omniverse info.
    */
    String m_sdfPathOmni = "";

   private:
    /*************************************************************************/
    /**
    * Members.
    */
    /*************************************************************************/

    /*
    * @brief .
    */
    SPtr<Actor> m_pActor;

    /*
    * @brief .
    */
    SPtr<SceneNode> m_pSelectedNode;

    /*
    * @brief .
    */
    SPtr<SceneNode> m_pParentNode;

    /*
    * @brief .
    */
    Vector<SPtr<SceneNode>> m_pChildNodes;
  };
}