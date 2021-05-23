#pragma once

#include "gaPrerequisitesCore.h"

namespace gaEngineSDK
{
  class GA_CORE_EXPORT GameObject 
  {
    public:
      /*************************************************************************/
      /**
      * Constructor and destructor.
      */
      /*************************************************************************/
      GameObject() = default;
      
      ~GameObject() = default;
    
      /***********************************************************************/
      /**
      * Virtual methods.
      */
      /***********************************************************************/

      /*
      * @brief
      */
      virtual void
      onInit() {};

      /*
      * @brief
      */
      virtual void
      onUpdate(const float& deltaTime) {};

      /*
      * @brief
      */
      virtual void
      onDelete() {};

      /*
      * @brief
      */
      virtual void
      onDraw() {};

      /***********************************************************************/
      /**
      * Sets and Gets.
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
      uint32
      getID();

      /*
      * @brief
      */
      void
      setLayer(uint32 layer = 0);

      /*
      * @brief
      */
      uint32
      getLayer();

      /*
      * @brief
      */
      void
      setParent(WeakSPtr<GameObject> parent);

      /*
      * @brief
      */
      SPtr<GameObject>
      getParent();
      
      /*
      * @brief
      */
      void
      setChildrens(Vector<SPtr<GameObject>> children);

      /*
      * @brief
      */
      Vector<SPtr<GameObject>>
      getChildrens();
      
      /***********************************************************************/
      /**
      * Members.
      */
      /***********************************************************************/

      /*
      * @brief
      */
      bool isSelected = false;

    protected:

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
  };
}