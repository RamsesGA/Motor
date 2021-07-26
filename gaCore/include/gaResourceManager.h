#pragma once

#include <gaModule.h>

#include "gaPrerequisitesCore.h"
#include "gaResource.h"
#include "gaComponent.h"
#include "gaAnimations.h"
#include "gaMesh.h"
#include "gaMaterials.h"

namespace gaEngineSDK {

  namespace TYPE_TEXTURES {
    enum E {
      kDiffuse = 0,
      kAlbedo,
      kMetallic,
      kNormal,
      kRoughness,
      kSpecular,
      kAO,
      kEmissive
    };
  }

  class Materials;

  class GA_CORE_EXPORT ResourceManager : public Component
  {
   public:
     /***********************************************************************/
     /**
     * Constructor and destructor.
     */
     /***********************************************************************/
     ResourceManager() = default;

     /*
     * @brief Function to initialize class members and start with assimp.
     * @param Variable string with the address of the file.
     * @param Variable with API information.
     */
     ResourceManager(const String& path);

     ~ResourceManager() = default;

     /***********************************************************************/
     /**
     * Inheritance methods.
     */
     /***********************************************************************/

     /*
     * @brief .
     */
     void
     update(const float& deltaTime)override;

     /*
     * @brief .
     */
     void
     render()override;
   
     /***********************************************************************/
     /**
     * Process methods.
     */
     /***********************************************************************/

     /*
     * @brief Function to process node information.
     * @param Assimp variable with node information
     * @param Assimp variable with scene information.
     * @param Variable with API information.
     */
     void
     processNode(aiNode* pANode);

     /*
     * @brief Function to process the information of the meshes.
     * @param Assimp variable with the mesh information.
     * @param Assimp variable with scene information.
     * @param Variable with API information.
     */
     Mesh
     processMesh(aiMesh* pAMesh);

     /*
     * @brief .
     */
     Mesh
     processBonesInfo(aiMesh* pAMesh, Vertex* vertexInfo, uint32 numVertexes, Mesh paramMesh);

     /*
     * @brief .
     */
     void
     processIndexInfo(uint32 numIndices);

     /*
     * @brief .
     */
     void
     processAnimationInfo();

     /************************************************************************/
     /**
     * Methods.
     */
     /************************************************************************/

     /*
     * @brief Function for loading the model texture.
     * @param Assimp variable for the material.
     * @param Assimp variable for the texture type.
     * @param Variable with API information.
     */
     void
     loadMaterialTextures(aiMaterial* pAMat, aiTextureType Atype);

     /*
     * @brief .
     */
     void
     loadModelNodes(WeakSPtr<ModelNodes> myNode, const aiNode* ainode);

     /*
     * @brief .
     */
     String
     deleteSlashs(String file);

     /*
     * @brief .
     */
     void
     searchingTextures(aiMesh * pAMesh);

     /*
     * @brief .
     */
     SPtr<Resource>
     loadingTextureFromFile(const RESOURCE_TYPES::E& type);

     /************************************************************************/
     /**
     * Gets.
     */
     /************************************************************************/

     /*
     * @brief Function to get the direction of the texture.
     * @param String with the file name.
     */
     String 
     getTexturePath(String file);

     /*
     * @brief .
     */
     String
     getModelDirectory();

     /*
     * @brief .
     */
     Vector<Textures*>
     getVecTextures();

     /*
     * @brief .
     */
     Vector<ConstBuffBonesTransform> m_meshBones;

   protected:
     /*
     * @brief .
     */
     void
     createDirectories(String const& path);

   private:
     /***********************************************************************/
     /**
     * Members.
     */
     /***********************************************************************/
     
     /*
     * @brief .
     */
     bool m_playAnimation = true;

     /*
     * @brief .
     */
     float m_timeOfAnimation = 0.0f;

     /*
     * @brief Member to store the full name of the folder.
     */
     String m_modelDirectory;

     /*
     * @brief Member to store the full name of the folder.
     */
     String m_texturesDirectory;

     /*
     * @brief .
     */
     Animations m_animInfo;

     /*
     * @brief Member to store a string of textures.
     */
     Vector<Textures*> m_vTextures;

     /*
     * @brief Member to store a string of Meshes.
     */
     Vector<Mesh> m_vMeshes;

     /*
     * @brief Member to store a .
     */
     Vector<Materials> m_vMaterials;

     /*
     * @brief .
     */
     Vector<SPtr<AnimationData>> m_vAnimationData;
  };
}