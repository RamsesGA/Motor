#pragma once

#include "gaPrerequisitesCore.h"
#include "gaGameObject.h"

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
    
      ~SceneNode() = default;
    
      /***********************************************************************/
      /**
      * Methods.
      */
      /***********************************************************************/
    
      /*
      * @brief
      */
      void
      onInit(String nodeName, WeakSPtr<SceneNode> nodeParent);
    
    private:
      /***********************************************************************/
      /**
      * Members.
      */
      /***********************************************************************/

      String m_nodeName;
    
      WeakSPtr<SceneNode> m_pNodeParent;

      Vector<WeakSPtr<SceneNode>> m_pNodesChild;

      WeakSPtr<GameObject> m_pGameObject;
  };
}