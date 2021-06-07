#include "gaResourceManager.h"
#include "gaGraphicsApi.h"
#include "gaMesh.h"

namespace gaEngineSDK {
  void 
  ResourceManager::initLoadModel(String const& path) {
    //Read _file via assimp
    m_pAScene = m_aImporter.ReadFile(path,
                                     aiProcessPreset_TargetRealtime_Fast |
                                     aiProcess_ConvertToLeftHanded |
                                     aiProcess_FindInstances |
                                     aiProcess_ValidateDataStructure |
                                     aiProcess_OptimizeMeshes |
                                     aiProcess_Debone);
    //Check for errors
    if (!m_pAScene || m_pAScene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !m_pAScene->mRootNode) {
      throw new std::exception("Error, al leer el path del modelo o el modelo no existe");
    }

    //Creamos el directorio del modelo y de su textura
    createDirectories(path);

    auto myGraphicApi = g_graphicApi().instancePtr();

    //Create texture sampler for model's textures
    m_newModel->setSamplers(myGraphicApi->createSamplerState());

    //Process assimp's root pNode recursively
    processNode(m_pAScene->mRootNode, m_pAScene);
  }

  void
  ResourceManager::processNode(aiNode* pANode, const aiScene* pAScene) {
    //Process each _mesh located at the current pNode
    for (uint32 i = 0; i < pANode->mNumMeshes; i++) {
      /*
      * The pNode object only contains indices to index the actual objects in the pAScene.
      * The pAScene contains all the data, pNode is just to keep stuff organized.
      */
      aiMesh* _mesh = pAScene->mMeshes[pANode->mMeshes[i]];
      m_pMeshes.push_back(processMesh(_mesh, pAScene));
    }
    for (uint32 i = 0; i < pANode->mNumChildren; i++) {
      processNode(pANode->mChildren[i], pAScene);
    }
  }

  Mesh* 
  ResourceManager::processMesh(aiMesh* pAMesh, const aiScene* pAScene) {
    //Data to fill
    Vector<uint32> indices;
    Vector<Vertex::E> vertices;
    Vector<Texture::E> textures;

    uint32 numIndex;
    uint32 numVertex = pAMesh->mNumVertices;
    String meshName = pAMesh->mName.C_Str();

    //Walk through each of the _mesh's vertices.
    for (uint32 i = 0; i < pAMesh->mNumVertices; i++) {
      Vertex::E structVertex;

      //Positions
      structVertex.position.m_x = pAMesh->mVertices[i].x;
      structVertex.position.m_y = pAMesh->mVertices[i].y;
      structVertex.position.m_z = pAMesh->mVertices[i].z;

      //Texcoords
      //Check if _mesh contains texcoords
      if (pAMesh->mTextureCoords[0]) {
        structVertex.texCoords.m_x = pAMesh->mTextureCoords[0][i].x;
        structVertex.texCoords.m_y = pAMesh->mTextureCoords[0][i].y;
      }
      else {
        structVertex.texCoords = Vector4(0.0f, 0.0f, 0.0f, 0.0f);
      }

      //Normals
      structVertex.normal.m_x = pAMesh->mNormals[i].x;
      structVertex.normal.m_y = pAMesh->mNormals[i].y;
      structVertex.normal.m_z = pAMesh->mNormals[i].z;

      //Tangents
      structVertex.tangent.m_x = pAMesh->mTangents[i].x;
      structVertex.tangent.m_y = pAMesh->mTangents[i].y;
      structVertex.tangent.m_z = pAMesh->mTangents[i].z;

      //Bitangents
      structVertex.tangent.m_x = pAMesh->mBitangents[i].x;
      structVertex.tangent.m_y = pAMesh->mBitangents[i].y;
      structVertex.tangent.m_z = pAMesh->mBitangents[i].z;

      vertices.push_back(structVertex);
    }

    //Go through each of the _mesh's faces and retrieve the corresponding indices.
    for (uint32 i = 0; i < pAMesh->mNumFaces; i++) {
      aiFace face = pAMesh->mFaces[i];

      //retrieve all indices of the face and store them in the indices vector
      for (uint32 j = 0; j < face.mNumIndices; j++) {
        indices.push_back(face.mIndices[j]);
      }
    }

    aiMaterial* material = pAScene->mMaterials[pAMesh->mMaterialIndex];

    Vector<Texture::E> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE);

    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

    Mesh* newMesh = new Mesh();
    newMesh->init(vertices, indices, textures);

    return newMesh;
  }

  Vector<Texture::E> 
  ResourceManager::loadMaterialTextures(aiMaterial* pAMat, aiTextureType Atype) {
    Vector<Texture::E> textures;

    for (uint32 i = 0; i < pAMat->GetTextureCount(Atype); i++) {
      aiString aistr;

      pAMat->GetTexture(Atype, i, &aistr);

      String srcFile = String(aistr.C_Str());
      srcFile = m_texturesDirectory + '/' + getTexturePath(srcFile);

      bool skip = false;

      for (uint32 j = 0; j < m_textures.size(); j++) {
        if (std::strcmp(m_textures[j].path.data(), srcFile.data()) == 0) {
          textures.push_back(m_textures[j]);
          skip = true;
          break;
        }
      }
      if (!skip) {
        auto myGraphicApi = g_graphicApi().instancePtr();

        Texture::E meshTexture;
        meshTexture.texture = myGraphicApi->loadTextureFromFile(srcFile);
        meshTexture.path = srcFile;
        textures.push_back(meshTexture);
        m_textures.push_back(meshTexture);
      }
    }

    return textures;
  }

  String 
  ResourceManager::getTexturePath(String file) {
    size_t realPos = 0;
    size_t posInvSlash = file.rfind('\\');
    size_t posSlash = file.rfind('/');

    if (posInvSlash == String::npos) {
      if (posSlash != String::npos) {
        realPos = posSlash;
      }
    }
    else {
      realPos = posInvSlash;

      if (!posSlash == String::npos) {
        if (posSlash > realPos) {
          posSlash = realPos;
        }
      }
    }

    return file.substr(realPos, file.length() - realPos);
  }

  Vector<Mesh*> 
  ResourceManager::getMeshes() {
    return m_pMeshes;
  }

  void
  ResourceManager::createDirectories(String const& path) {
    //Retrieve the directory path of the _file
    m_modelDirectory = path.substr(0, path.find_last_of('/'));

    //Esto es para obtener el path de las texturas.
    String miniPaht;
    for (uint32 i = 0; i < m_modelDirectory.size(); i++) {
      miniPaht += m_modelDirectory.at(i);
      if ("data/models/" == miniPaht) {
        m_texturesDirectory = miniPaht;
        m_texturesDirectory.replace(5, 9, "textures/");
        miniPaht.clear();
      }
    }
    m_texturesDirectory += miniPaht;
    miniPaht.clear();
  }
}