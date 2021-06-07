#include <gaVector2.h>

#include "gaModels.h"
#include "gaGraphicsApi.h"
#include "gaMesh.h"

namespace gaEngineSDK {

  void
  Model::init(String const& path) {
    //Read _file via assimp
    Assimp::Importer importer;

    const aiScene* pScene = importer.ReadFile(path,
      aiProcessPreset_TargetRealtime_Fast |
      aiProcess_ConvertToLeftHanded |
      aiProcess_FindInstances |
      aiProcess_ValidateDataStructure |
      aiProcess_OptimizeMeshes |
      aiProcess_Debone);

    //Check for errors
    if (!pScene || pScene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !pScene->mRootNode) {
      throw new std::exception("Error, al leer el path del modelo");
    }

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

    auto myGraphicApi = g_graphicApi().instancePtr();

    //Create texture sampler for model's textures
    m_pSamplers.push_back(myGraphicApi->createSamplerState());

    //Process assimp's root pNode recursively
    processNode(pScene->mRootNode, pScene);
  }

  void
  Model::draw() {
    auto myGraphicApi = g_graphicApi().instancePtr();

    if (nullptr == myGraphicApi) {
      throw new std::exception("Error, parametro nulo en Draw del modelo");
    }

    for (uint32 i = 0; i < m_pMeshes.size(); i++) {
      m_pMeshes[i]->draw(m_pSamplers);
    }
  }

  void
  Model::processNode(aiNode* pNode, const aiScene* pScene) {
    //Process each _mesh located at the current pNode
    for (uint32 i = 0; i < pNode->mNumMeshes; i++) {
      //The pNode object only contains indices to index the actual objects in the pScene.
      //The pScene contains all the data, pNode is just to keep stuff organized.
      aiMesh* _mesh = pScene->mMeshes[pNode->mMeshes[i]];
      m_pMeshes.push_back(processMesh(_mesh, pScene));
    }

    for (uint32 i = 0; i < pNode->mNumChildren; i++) {
      processNode(pNode->mChildren[i], pScene);
    }
  }

  Mesh*
  Model::processMesh(aiMesh* pMesh, const aiScene* pScene) {
    //Data to fill
    Vector<Vertex::E> vertices;
    Vector<uint32> indices;
    Vector<Texture::E> textures;

    //Walk through each of the _mesh's vertices.
    for (uint32 i = 0; i < pMesh->mNumVertices; i++) {
      Vertex::E structVertex;

      //Positions
      structVertex.position.m_x = pMesh->mVertices[i].x;
      structVertex.position.m_y = pMesh->mVertices[i].y;
      structVertex.position.m_z = pMesh->mVertices[i].z;

      //Texcoords
      //Check if _mesh contains texcoords
      if (pMesh->mTextureCoords[0]) {
        structVertex.texCoords.m_x = pMesh->mTextureCoords[0][i].x;
        structVertex.texCoords.m_y = pMesh->mTextureCoords[0][i].y;
      }
      else {
        structVertex.texCoords = Vector4(0.0f, 0.0f, 0.0f, 0.0f);
      }

      //Normals
      structVertex.normal.m_x = pMesh->mNormals[i].x;
      structVertex.normal.m_y = pMesh->mNormals[i].y;
      structVertex.normal.m_z = pMesh->mNormals[i].z;

      //Tangents
      structVertex.tangent.m_x = pMesh->mTangents[i].x;
      structVertex.tangent.m_y = pMesh->mTangents[i].y;
      structVertex.tangent.m_z = pMesh->mTangents[i].z;

      //Bitangents
      structVertex.tangent.m_x = pMesh->mBitangents[i].x;
      structVertex.tangent.m_y = pMesh->mBitangents[i].y;
      structVertex.tangent.m_z = pMesh->mBitangents[i].z;

      vertices.push_back(structVertex);
    }

    //Go through each of the _mesh's faces and retrieve the corresponding indices.
    for (uint32 i = 0; i < pMesh->mNumFaces; i++) {
      aiFace face = pMesh->mFaces[i];

      //retrieve all indices of the face and store them in the indices vector
      for (uint32 j = 0; j < face.mNumIndices; j++) {
        indices.push_back(face.mIndices[j]);
      }
    }

    aiMaterial* material = pScene->mMaterials[pMesh->mMaterialIndex];

    Vector<Texture::E> diffuseMaps = loadMaterialTextures(material,
      aiTextureType_DIFFUSE);

    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

    Mesh* newMesh = new Mesh();
    newMesh->init(vertices, indices, textures);

    return newMesh;
  }

  Vector<Texture::E>
  Model::loadMaterialTextures(aiMaterial* pMat, aiTextureType type) {
    Vector<Texture::E> textures;

    for (uint32 i = 0; i < pMat->GetTextureCount(type); i++) {
      aiString aistr;

      pMat->GetTexture(type, i, &aistr);

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
  Model::getTexturePath(String file) {
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

  void
  Model::setSamplers(SamplerState* sampler) {
    m_pSamplers.push_back(sampler);
  }
}