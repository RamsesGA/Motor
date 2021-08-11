#pragma once

#include "gaPrerequisitesCore.h"
#include "gaComponent.h"
#include "gaAnimations.h"
#include "gaMesh.h"

namespace gaEngineSDK {
  class Materials;
  class Model;

  class GA_CORE_EXPORT StaticMesh : public Component
  {
  public:
    /*************************************************************************/
    /*
    * Constructor and destructor.
    */
    /*************************************************************************/
    StaticMesh() = default;

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
    SPtr<Model> m_pModel;

    /*
    * @brief NO FRIEGUES RAM .
    */

    Animations m_animInfo;

    /*
    * @brief Member to store a string of Meshes.
    */
    Vector<Mesh> m_vMeshes;

    /*
    * @brief .
    */
    Vector<ConstBuffBonesTransform> m_vMeshBones;
    /*
    * @brief .
    */
    Vector<SPtr<AnimationData>> m_vAnimationData;

    /*
    */
    Map<String, SPtr<Materials>> m_mMaterials;

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