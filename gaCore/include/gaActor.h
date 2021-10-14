#pragma once

#include <gaTransform.h>

#include "gaPrerequisitesCore.h"
#include "gaComponent.h"

namespace gaEngineSDK {
  using std::dynamic_pointer_cast;
  using std::make_shared;

  class GA_CORE_EXPORT Actor
  {
   public:
    /*************************************************************************/
    /**
    * Constructor and destructor.
    */
    /*************************************************************************/
    
    Actor(String actorName);
    
    ~Actor() = default;
    
    /*************************************************************************/
    /**
    * Methods.
    */
    /*************************************************************************/
    
    void
    removeComponent();
    
    /*
    * @brief Actor information update.
    * @param deltaTime, time elapsed since the last frame.
    */
    void
    actorUpdate(const float& deltaTime);
    
    void
    actorRender();
    
    /*************************************************************************/
    /**
    * Gets.
    */
    /*************************************************************************/
    
    bool
    getIsSelected();
    
    String
    getActorName();

    template<typename T> 
    SPtr<T>
    getComponent(){
      for (auto component : m_vComponents) {
        auto result = dynamic_pointer_cast<T>(component);

        if (result) {
          return SPtr<T>(result);
        }
      }

      return nullptr;
    };
    
    /*************************************************************************/
    /**
    * Sets.
    */
    /*************************************************************************/
    
    void
    setIsSelected(bool isSelect);
    
    void
    setActorName(String name);
    
    void
    setComponent(WeakSPtr<Component> compoInfo);
    
   protected:
    /*************************************************************************/
    /**
    * Members.
    */
    /*************************************************************************/

    /*
    * @brief Bool to indicate if the actor is selected.
    */
    bool m_isSelected = false;

    /*
    * @brief String with the actor's name.
    */
    String m_actorName;

    /*
    * @brief .
    */
    SPtr<Actor> m_pParent;

    /*
    * @brief Vector of pointers to received component.
    */
    Vector<SPtr<Component>> m_vComponents;

    friend class Transform;
  };

}