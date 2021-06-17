#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/cimport.h>
#include <gaModule.h>

#include "gaPrerequisitesCore.h"
#include "gaModels.h"
#include "gaResource.h"

namespace gaEngineSDK {
  class GA_CORE_EXPORT ResourceManager : public Module <ResourceManager>
  {
    public:
      /***********************************************************************/
      /**
      * Constructor and destructor.
      */
      /***********************************************************************/
      ResourceManager() = default;
    
      ~ResourceManager() = default;
    
      /***********************************************************************/
      /**
      * Methods.
      */
      /***********************************************************************/
    
      /*
      * @brief Function to initialize class members and start with assimp.
      * @param Variable string with the address of the file.
      * @param Variable with API information.
      */
      void
      initLoadModel(String const& path);

      /*
      * @brief Function to process node information.
      * @param Assimp variable with node information
      * @param Assimp variable with scene information.
      * @param Variable with API information.
      */
      void
      processNode(aiNode* pANode, const aiScene* pAScene);

      /*
      * @brief Function to process the information of the meshes.
      * @param Assimp variable with the mesh information.
      * @param Assimp variable with scene information.
      * @param Variable with API information.
      */
      Mesh* 
      processMesh(aiMesh* pAMesh, const aiScene* pAScene);

      void
      processBonesInfo(aiMesh* pAMesh, Vertex* structureVertex, uint32 numVertexes);

      void
      processIndexInfo(uint32 numIndices);

      void
      processAnimationInfo();

      /*
      * @brief Function for loading the model texture.
      * @param Assimp variable for the material.
      * @param Assimp variable for the texture type.
      * @param Variable with API information.
      */
      Vector<Texture>
      loadMaterialTextures(aiMaterial* pAMat, aiTextureType Atype);

      /*
      * @brief .
      */
      void
      loadModelNodes(WeakSPtr<ModelNodes> myNode, const aiNode* ainode);

      /***********************************************************************/
      /**
      * Gets.
      */
      /***********************************************************************/

      /*
      * @brief Function to get the direction of the texture.
      * @param String with the file name.
      */
      String 
      getTexturePath(String file);

      /*
      * @brief .
      */
      Vector<Mesh*>
      getMeshes();

      /*
      * @brief .
      */
      Vector<SamplerState*>
      getSamplerInfo();

      Vector<SPtr<Model>>
      getVecModels();
      
      Mesh*
      getMesh();

      SPtr<Model>
      getModel();

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
      Assimp::Importer m_aImporter;

      /*
      * @brief .
      */
      const aiScene* m_pAScene;

      /*
      * @brief .
      */
      Mesh* m_newMesh = nullptr;

      /*
      * @brief .
      */
      SPtr<Model> m_newModel;

      /*
      * @brief Member to store a string of Meshes.
      */
      Vector<Mesh*> m_pVMeshes;

      /*
      * @brief Member to store a string of textures.
      */
      Vector<Texture> m_textures;

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
      Vector<SPtr<Model>> m_vModels;
  };

  GA_CORE_EXPORT ResourceManager&
  g_resourceManager();
}