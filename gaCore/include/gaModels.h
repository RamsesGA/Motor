#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/cimport.h>

#include "gaPrerequisitesCore.h"
#include "gaMesh.h"
#include "gaResource.h"

namespace gaEngineSDK {

  class Textures;

  struct Key {
    float m_time;
    //TODO: Transformar a quaternion el tipo de dato y si hay errores crear
    //la estructura de key de rotación con un cuaternión en lugar de un vec4
    Vector4 m_value;
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
    Vector<Key> m_RotationKeys;


    /*
    * @brief .
    */
    uint32 m_numRotationKeys;

    /*
    * @brief .
    */
    Vector<Key> m_PositionKeys;

    /*
    * @brief .
    */
    uint32 m_numPositionKeys;
  };

  struct AnimationData {
    /*
    * @brief .
    */
    String m_animationName;

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
    uint32 m_channels;

    /*
    * @brief .
    */
    Vector<SPtr<AnimationNode>> m_vChannels;
  };

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
      * Herency methods.
      */
      /***********************************************************************/

      /*
      * @brief
      */
      void
      onInit() override {};

      /*
      * @brief
      */
      void
      onUpdate(const float& deltaTime) override {};

      /*
      * @brief
      */
      void
      onDelete() override {};

      /*
      * @brief
      */
      void
      onRelease() override {};

      /***********************************************************************/
      /*
      * Methods.
      */
      /***********************************************************************/

      /*
      * @brief Function to initialize class members and start with assimp.
      * @param Variable string with the address of the file.
      * @param Variable with API information.
      */
      void 
      init(String const& path, GraphicsApi* pGraphicApi);

      /*
      * @brief Function to draw based on mmeshes.
      * @param Variable with API information.
      */
      void 
      draw(GraphicsApi* pGraphicApi);

      /*
      * @brief Function to process node information.
      * @param Assimp variable with node information
      * @param Assimp variable with scene information.
      * @param Variable with API information.
      */
      void 
      processNode(aiNode* pNode, const aiScene* pScene, GraphicsApi* pGraphicApi);

      /*
      * @brief Function to process the information of the meshes.
      * @param Assimp variable with the mesh information.
      * @param Assimp variable with scene information.
      * @param Variable with API information.
      */
      Mesh* 
      processMesh(aiMesh* pMesh, const aiScene* pScene, GraphicsApi* pGraphicApi);

      /*
      * @brief Function for loading the model texture.
      * @param Assimp variable for the material.
      * @param Assimp variable for the texture type.
      * @param Variable with API information.
      */
      Vector<Texture::E>
      loadMaterialTextures(aiMaterial* pMat, aiTextureType type, GraphicsApi* pGraphicApi);

      /*
      * @brief Function to get the direction of the texture.
      * @param String with the file name.
      */
      String 
      getTexturePath(String file);

      /***********************************************************************/
      /*
      * Members.
      */
      /***********************************************************************/

      /*
      * @brief .
      */
      Vector<SPtr<Mesh>> m_vMeshes;

      /*
      * @brief .
      */
      Vector<SPtr<AnimationData>> m_vAnimationData;

    private:
      /***********************************************************************/
      /*
      * Members.
      */
      /***********************************************************************/
    
      /*
      * @brief Member to store the full name of the folder.
      */
      String m_directory;
      
      /*
      * @brief Member to store a string of samplers.
      */
      Vector <SamplerState*> m_pSampler;
      
      /*
      * @brief Member to store a string of Meshes.
      */
      Vector<Mesh*> m_pMeshes;
      
      /*
      * @brief Member to store a string of textures.
      */
      Vector<Texture::E> m_textures;

      /*
      * @brief .
      */
      Matrix4x4 m_globalInverseTransform;

      /*
      * @brief .
      */
      SPtr<AnimationData> m_currentAnimation = nullptr;

      /*
      * @brief .
      */
      SPtr<ModelNodes> m_modelNodes = nullptr;

      /*
      * @brief .
      */
      uint32 m_numAnimations;
  };  
}