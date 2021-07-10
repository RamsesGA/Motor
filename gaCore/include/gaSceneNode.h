#pragma once

#include "gaPrerequisitesCore.h"
#include "gaActor.h"

namespace gaEngineSDK {
  class SceneNode
  {
    public:
      /***********************************************************************/
      /**
      * Constructor and destructor.
      */
      /***********************************************************************/
      SceneNode() = default;

      SceneNode(WeakSPtr<Actor> actor, WeakSPtr<SceneNode> parent);
    
      ~SceneNode() = default;
    
      /***********************************************************************/
      /**
      * Methods.
      */
      /***********************************************************************/

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

      /***********************************************************************/
      /**
      * Sets.
      */
      /***********************************************************************/

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

      /***********************************************************************/
      /**
      * Gets.
      */
      /***********************************************************************/

      /*
      * @brief .
      */
      SPtr<Actor>
      getActorNode();

      /*
      * @brief .
      */
      WeakSPtr<SceneNode>
      getParentNode();

      /*
      * @brief .
      */
      Vector<SPtr<SceneNode>>
      getChildNodes();
    
    private:
      /***********************************************************************/
      /**
      * Members.
      */
      /***********************************************************************/

      /*
      * @brief .
      */
      SPtr<Actor> m_pActor = nullptr;

      /*
      * @brief .
      */
      SPtr<SceneNode> m_pSelectedNode = nullptr;

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