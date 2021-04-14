#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/cimport.h>

#include "gaPrerequisitesCore.h"
#include "gaMesh.h"

namespace gaEngineSDK {

  class Textures;

  class GA_CORE_EXPORT Model
  {
    public:
      /***********************************************************************/
      /*
      * Constructor and destructor.
      */
      /***********************************************************************/
      Model() = default;

      virtual 
      ~Model() = default;

      /***********************************************************************/
      /*
      * Methods.
      */
      /***********************************************************************/

      /*
      * @brief Function to initialize class members and start with assimp.
      * @param 
      */
      void 
      init(String const& path, GraphicsApi* pGraphicApi);

      /*
      * @brief Function to draw based on mmeshes.
      * @param 
      */
      void 
      draw(GraphicsApi* pGraphicApi);

      /*
      * @brief Function to process node information.
      * @param
      */
      void 
      processNode(aiNode* pNode, const aiScene* pScene, GraphicsApi* pGraphicApi);

      /*
      * @brief Function to process the information of the meshes.
      * @param
      */
      Mesh* 
      processMesh(aiMesh* pMesh, const aiScene* pScene, GraphicsApi* pGraphicApi);

      /*
      * @brief Function for loading the model texture.
      * @param
      */
      Vector<Texture::E>
      loadMaterialTextures(aiMaterial* pMat, aiTextureType type, GraphicsApi* pGraphicApi);

      /*
      * @brief Function to get the direction of the texture.
      * @param
      */
      String 
      getTexturePath(String file);

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
  };
}