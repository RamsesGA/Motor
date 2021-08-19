#pragma once

#include <gaTransform.h>

#include "gaPrerequisitesCore.h"
#include "gaGameObject.h"
#include "gaComponent.h"

namespace gaEngineSDK {
  class GA_CORE_EXPORT Actor final : public GameObject
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
    removeComponent() override;
    
    /*
    * @brief Actor information update.
    * @param deltaTime, time elapsed since the last frame.
    */
    void
    actorUpdate(const float& deltaTime);
    
    void
    actorRender() override;
    
    /*************************************************************************/
    /**
    * Gets.
    */
    /*************************************************************************/
    
    bool
    getIsSelected() override;
    
    String
    getActorName() override;
    
    /*************************************************************************/
    /**
    * Sets.
    */
    /*************************************************************************/
    
    void
    setIsSelected(bool isSelect) override;
    
    void
    setActorName(String name) override;
    
    void
    setComponent(WeakSPtr<Component> compoInfo) override;
    
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
    * @brief Transformation component.
    */
    SPtr<Transform> m_actorTransform;

    /*
    * @brief Vector of pointers to received component.
    */
    Vector<SPtr<Component>> m_vComponents;
  };
}