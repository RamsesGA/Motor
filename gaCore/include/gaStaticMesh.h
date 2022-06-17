#pragma once
#pragma warning( disable: 4251 )

#include "gaPrerequisitesCore.h"
#include "gaComponent.h"
#include "gaAnimations.h"

namespace gaEngineSDK {
  class Materials;
  class Models;
  class Mesh;

  class GA_CORE_EXPORT StaticMesh : public Component
  {
   public:
    /*************************************************************************/
    /*
    * Constructor and destructor.
    */
    /*************************************************************************/
    StaticMesh() = default;

    StaticMesh(WeakSPtr<Models> newModel);

    ~StaticMesh() = default;

    /*************************************************************************/
    /**
    * Inheritance methods.
    */
    /*************************************************************************/

    /*
    * @brief .
    */
    void
    update(const float& deltaTime) override;

    /*
    * @brief .
    */
    void
    render() override;

    /*************************************************************************/
    /*
    * Members.
    */
    /*************************************************************************/

    /*
    * @brief SI.
    */
    SPtr<Models> m_pModel;

   private:
    /*
    * @brief .
    */
    bool m_playAnimation = true;

    /*
    * @brief .
    */
    float m_timeOfAnimation = 0.0f;
  };
}