#pragma once

#include "gaPrerequisitesCore.h"
#include "gaComponent.h"

namespace gaEngineSDK
{
  class GA_CORE_EXPORT GameObject 
  {
    public:
      /***********************************************************************/
      /**
      * Constructor and destructor.
      */
      /***********************************************************************/
      GameObject() = default;
      
      ~GameObject() = default;
    
      /***********************************************************************/
      /**
      * Methods.
      */
      /***********************************************************************/

      /*
      * @brief
      */
      virtual void
      update(const float& deltaTime);

      /*
      * @brief
      */
      virtual void
      draw();

      SPtr<Component>
      addComponent(TYPE_COMPONENTS::E typeComponent, SPtr<Component> component = nullptr);

      void
      removeComponent(TYPE_COMPONENTS::E typeComponent);

      /***********************************************************************/
      /**
      * Gets.
      */
      /***********************************************************************/

      /*
      * @brief
      */
      uint32
      getID();

      /*
      * @brief
      */
      uint32
      getLayer();

      /*
      * @brief
      */
      SPtr<GameObject>
      getParent();

      /*
      * @brief
      */
      Vector<SPtr<GameObject>>
      getChildrens();

      /*
      * @brief
      */
      bool
      getIsSelected();

      SPtr<Component>
      getComponent(TYPE_COMPONENTS::E typeComponent);

      /***********************************************************************/
      /**
      * Sets.
      */
      /***********************************************************************/

      /*
      * @brief
      */
      void
      setID(uint32 id);

      /*
      * @brief
      */
      void
      setLayer(uint32 layer = 0);

      /*
      * @brief
      */
      void
      setParent(WeakSPtr<GameObject> parent);
      
      /*
      * @brief
      */
      void
      setChildrens(Vector<SPtr<GameObject>> children);

      /*
      * @brief
      */
      void
      setIsSelected(bool isSelect);
      

    protected:
      /***********************************************************************/
      /**
      * Members.
      */
      /***********************************************************************/

      /*
      * @brief
      */
      bool m_isSelected = false;

      /*
      * @brief
      */
      uint32 m_id = 0;

      /*
      * @brief
      */
      uint32 m_layer = 0;

      /*
      * @brief
      */
      SPtr<GameObject> m_parent = nullptr;

      /*
      * @brief
      */
      Vector<SPtr<GameObject>> m_vChildrens;

      /*
      * @brief
      */
      Map<TYPE_COMPONENTS::E, SPtr<Component>> m_mapComponents;
  };
}