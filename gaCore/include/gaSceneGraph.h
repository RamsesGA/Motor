#pragma once

#include <gaModule.h>

#include "gaPrerequisitesCore.h"
#include "gaGameObject.h"
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
      onInit(String nameRoot);

      /*
      * @brief
      */
      void
      onUpdate(const float& deltaTime);

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

      ///*
      //* @brief
      //*/
      Vector <SPtr<GameObject>> m_vGameObjects;

      ///*
      //* @brief
      //*/
      SPtr<GameObject> m_selectedObj = nullptr;

      ///*
      //* @brief sceneNode?
      //*/
      SPtr<GameObject> m_root = nullptr;

      /*
      * @brief
      */
      SPtr<SceneNode> m_rootNode = nullptr;
  };
}