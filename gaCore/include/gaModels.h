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