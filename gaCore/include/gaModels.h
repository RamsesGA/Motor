#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/cimport.h>
#include <gaQuaternions.h>

#include "gaPrerequisitesCore.h"
#include "gaSamplerState.h"

namespace gaEngineSDK {

  class Textures;

  struct Key {
    float m_time;
    Vector4 m_value;
  };

  struct RotationKey {
    Quaternions m_value;
    float m_time;
  };

  struct ModelNodes {
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
    * @brief Member to define all the childrens.
    */
    Vector<SPtr<ModelNodes>> m_vChildrenNodes;

    /*
    * @brief Member to save the quantity of children nodes.
    */
    uint32 m_numChildrens;

    /*
    * @brief Member to save the quantity of meshes.
    */
    uint32 m_numMeshes;
  };

  struct AnimationNode {
    /*
    * @brief Mmember to know what.
    */
    String m_nodeName;

    /*
    * @brief .
    */
    Vector<Key> m_scalingKeys;

    /*
    * @brief .
    */
    uint32 m_numScalingKeys;

    /*
    * @brief .
    */
    Vector<RotationKey> m_vRotationKeys;

    /*
    * @brief .
    */
    uint32 m_numRotationKeys;

    /*
    * @brief .
    */
    Vector<Key> m_vPositionKeys;

    /*
    * @brief .
    */
    uint32 m_numPositionKeys;
  };

  struct AnimationData {
    /*
    * @brief .
    */
    String m_animationName = "Default Animation";

    /*
    * @brief .
    */
    float m_animDuration;

    /*
    * @brief .
    */
    float m_ticksPerSecond;

    /*
    * @brief .
    */
    uint32 m_numChannels;

    /*
    * @brief .
    */
    Vector<SPtr<AnimationNode>> m_vChannels;
  };

  class ResourceManager;
  class Mesh;

  class GA_CORE_EXPORT Model : public Resource
  {
    public:
      /***********************************************************************/
      /*
      * Constructor and destructor.
      */
      /***********************************************************************/
      Model() = default;

      ~Model() { onRelease(); };

      /***********************************************************************/
      /*
      * Methods.
      */
      /***********************************************************************/

      void
      setSamplers(SamplerState* sampler);

      Vector<SamplerState*>
      getSamplerInfo();

      /***********************************************************************/
      /*
      * Members.
      */
      /***********************************************************************/

      /*
      * @brief .
      */
      uint32 m_numAnimations = 0;

      /*
      * @brief .
      */
      Vector<SPtr<Mesh>> m_vMeshes;

      /*
      * @brief .
      */
      Vector<SPtr<AnimationData>> m_vAnimationData;

      /*
      * @brief .
      */
      Matrix4x4 m_globalInverseTransform;

      /*
      * @brief .
      */
      SPtr<ModelNodes> m_modelNodes = nullptr;

      /*
      * @brief .
      */
      Vector<char*> m_pAnimationsList;

    private:
      /***********************************************************************/
      /*
      * Members.
      */
      /***********************************************************************/

      /*
      * @brief .
      */
      SPtr<AnimationData> m_currentAnimation = nullptr;

      /*
      * @brief .
      */
      ResourceManager* m_resourceManager = nullptr;

      /*
      * @brief Member to store a string of samplers.
      */
      Vector<SamplerState*> m_pSamplers;
  };  
}