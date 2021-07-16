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
    float m_time;
    Quaternions m_value;
  };

  struct ModelNodes {
    /*
    * @brief Member to save the quantity of children nodes.
    */
    uint32 m_numChildrens;

    /*
    * @brief Member to save the quantity of meshes.
    */
    uint32 m_numMeshes;

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
    uint32 m_numScalingKeys;

    /*
    * @brief .
    */
    uint32 m_numRotationKeys;

    /*
    * @brief .
    */
    uint32 m_numPositionKeys;
    
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
    uint32 m_numChannels;

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
    String m_animationName = "Default Animation";

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

      ~Model() = default;

      /***********************************************************************/
      /*
      * Gets.
      */
      /***********************************************************************/

      /*
      * @brief .
      */
      uint32
      getNumAnims();

      /*
      * @brief .
      */
      Vector<String>
      getAnimList();

      /*
      * @brief .
      */
      SPtr<SamplerState>
      getSampler();

      /*
      * @brief .
      */
      SPtr<ResourceManager>
      getResourceMgrInfo();

      /*
      * @brief .
      */
      Vector<SPtr<Mesh>>
      getMeshesInfo();

      /***********************************************************************/
      /*
      * Sets.
      */
      /***********************************************************************/

      /*
      * @brief .
      */
      void
      setNumAnims(uint32 numAnims);

      /*
      * @brief .
      */
      void
      setAnimList(String animName);

      /*
      * @brief .
      */
      void
      setSampler(WeakSPtr<SamplerState> sampler);

      /*
      * @brief .
      */
      void
      setResourceMgrInfo(WeakSPtr<ResourceManager> resourceMgr);

      /*
      * @brief .
      */
      void
      setMeshesInfo(SPtr<Mesh> meshInfo);

      /*
      * @brief .
      */
      Matrix4x4 m_globalInverseTransform;

      /*
      * @brief .
      */
      SPtr<ModelNodes> m_modelNodes = nullptr;

    private:
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
      Vector<String> m_animationsList;

      /*
      * @brief Member to store a string of samplers.
      */
      SPtr<SamplerState> m_sampler = nullptr;

      /*
      * @brief .
      */
      SPtr<ResourceManager> m_resourceManager = nullptr;

      /*
      * @brief .
      */
      Vector<SPtr<Mesh>> m_vMeshes;
  };  
}