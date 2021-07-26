#pragma once

#include <gaQuaternions.h>

#include "gaPrerequisitesCore.h"

namespace gaEngineSDK {
  struct Key {
    float m_time = 0.0f;
    Vector4 m_value;
  };

  struct RotationKey {
    float m_time = 0.0f;
    Quaternions m_value;
  };

  struct ModelNodes {
    /*
    * @brief Member to save the quantity of children nodes.
    */
    uint32 m_numChildrens = 0;

    /*
    * @brief Member to save the quantity of meshes.
    */
    uint32 m_numMeshes = 0;

    /*
    * @brief Member to define the name.
    */
    String m_name;

    /*
    * @brief Member to define the transform matrix.
    */
    Matrix4x4 m_transformMatrix;

    /*
    * @brief Member to define the first node.
    */
    SPtr<ModelNodes> m_parentNode = nullptr;

    /*
    * @brief Member to define all the children.
    */
    Vector<SPtr<ModelNodes>> m_vChildrenNodes;
  };

  struct AnimationNode {
    /*
    * @brief .
    */
    uint32 m_numScalingKeys = 0;

    /*
    * @brief .
    */
    uint32 m_numRotationKeys = 0;

    /*
    * @brief .
    */
    uint32 m_numPositionKeys = 0;

    /*
    * @brief Member to know what.
    */
    String m_nodeName;

    /*
    * @brief .
    */
    Vector<Key> m_scalingKeys;

    /*
    * @brief .
    */
    Vector<RotationKey> m_vRotationKeys;

    /*
    * @brief .
    */
    Vector<Key> m_vPositionKeys;
  };

  struct AnimationData {
    /*
    * @brief .
    */
    uint32 m_numChannels = 0;

    /*
    * @brief .
    */
    float m_animDuration = 0.0f;

    /*
    * @brief .
    */
    float m_ticksPerSecond = 0.0f;

    /*
    * @brief .
    */
    String m_animationName = "Default Animation";

    /*
    * @brief .
    */
    Vector<SPtr<AnimationNode>> m_vChannels;
  };

  class Animations
  {
   public:
    /***********************************************************************/
    /*
    * Constructor and destructor.
    */
    /***********************************************************************/
    Animations() = default;

    ~Animations() = default;

    /***********************************************************************/
    /*
    * Methods.
    */
    /***********************************************************************/

    /*
    * @brief .
    */
    void
    setAnimName(String animName);

    /*
    * @brief .
    */
    void
    setNumAnims(uint32 numAnims);

    /***********************************************************************/
    /*
    * Members.
    */
    /***********************************************************************/

    /*
    * @brief .
    */
    SPtr<ModelNodes> m_pStrModelNodes;

   private:
    /*
    * @brief .
    */
    uint32 m_numAnimations = 0;

    /*
    * @brief .
    */
    Vector<String> m_vAnimationsNames;
    
  };
}