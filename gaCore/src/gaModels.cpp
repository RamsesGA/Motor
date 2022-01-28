#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/cimport.h>

#include "gaModels.h"
#include "gaGraphicsApi.h"

using std::make_shared;
using std::pair;

namespace gaEngineSDK {
  String g_texturesDirectory;

  Animations g_animInfo;

  Vector<Mesh> g_vMeshes;

  Vector<String> g_vMeshName;

  Vector<Textures*> g_vTextures;

  Vector<SPtr<AnimationData>> g_vAnimationData;

  Map<String, SPtr<Materials>> g_mMaterials;

  /***************************************************************************/
  /**
  * Local functions.
  */
  /***************************************************************************/

  /*
  * @brief Function to process node information.
  * @param Assimp variable with node information
  * @param Assimp variable with scene information.
  */
  void
  processNode(aiNode* pANode, const aiScene* pAScene);

  /*
  * @brief Function to process the information of the meshes.
  * @param Assimp variable with the mesh information.
  * @param Assimp variable with scene information.
  */
  Mesh
  processMesh(aiMesh* pAMesh, const aiScene* pAScene);

  /*
  * @brief .
  */
  Mesh
  processBonesInfo(aiMesh* pAMesh, Vertex* vertexInfo, uint32 numVertexes, Mesh paramMesh);

  /*
  * @brief .
  */
  void
  processAnimationInfo(const aiScene* pAScene);

  /*
  * @brief .
  */
  void
  loadModelNodes(WeakSPtr<ModelNodes> myNode, const aiNode* ainode);

  /*
  * @brief .
  */
  String
  obtainTexturePath(String file);

  /*
  * @brief .
  */
  String
  deleteDobleSlash(String file);

  /*
  * @brief .
  */
  void
  searchTextures(aiMesh* pAMesh, const aiScene* pAScene);

  /***************************************************************************/
  /**
  * Inheritance.
  */
  /***************************************************************************/

  void
  Models::loadFromFile(const String& file) {
    Assimp::Importer aImporter;
    const aiScene* pAScene = nullptr;

    //Read _file via assimp
    pAScene = aImporter.ReadFile(file, 
                                 aiProcessPreset_TargetRealtime_MaxQuality |
                                 aiProcess_ConvertToLeftHanded | 
                                 aiProcess_Triangulate);

    //Check for errors
    if (!pAScene || pAScene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !pAScene->mRootNode) {
      return;
    }

    //We create the directory of the model and its texture
    createDirectories(file);

    g_animInfo = m_animInfo;
    g_vAnimationData =m_vAnimationData;
    g_vMeshes.resize(m_vModelData.size());
    g_vMeshes = m_vMeshes;
    g_mMaterials = m_mMaterials;
    g_vMeshName = m_vMeshName;

    //Process assimp's root pNode recursively
    processNode(pAScene->mRootNode, pAScene);

    processAnimationInfo(pAScene);

    m_animInfo = g_animInfo;
    m_vAnimationData = g_vAnimationData;
    m_vModelData.resize(g_vMeshes.size());
    m_vMeshes = g_vMeshes;
    m_mMaterials = g_mMaterials;
    m_vMeshName = g_vMeshName;
  }

  void
  Models::addNewMesh(Mesh newMesh) {
    m_vMeshes.push_back(newMesh);
  }

  Mesh&
  Models::getMesh(uint32 index) {
    return m_vMeshes.at(index);
  }

  uint32
  Models::getSizeMeshes() {
    return (uint32)m_vMeshes.size();
  }

  /***************************************************************************/
  /**
  * Local functions.
  */
  /***************************************************************************/

  void 
  processNode(aiNode* pANode, const aiScene* pAScene) {
    //Process each _mesh located at the current pNode
    uint32 tempNumMeshes = pANode->mNumMeshes;

    for (uint32 i = 0; i < tempNumMeshes; ++i) {
      /*
      * The pNode object only contains indices to index the actual objects in the pAScene.
      * The pAScene contains all the data, pNode is just to keep stuff organized.
      */
      aiMesh* _mesh = pAScene->mMeshes[pANode->mMeshes[i]];
      g_vMeshes.push_back(processMesh(_mesh, pAScene));
    }


    uint32 tempNumChildren = pANode->mNumChildren;
    for (uint32 i = 0; i < tempNumChildren; ++i) {
      processNode(pANode->mChildren[i], pAScene);
    }
  }

  Mesh 
  processMesh(aiMesh* pAMesh, const aiScene* pAScene) {
    //Data to fill
    Vector<uint32> indices;
    Vector<Vertex> vVertices;

    Mesh newMesh;

    vVertices.resize(pAMesh->mNumVertices);

    if (!pAMesh->HasFaces()) {
      return newMesh;
    }

    //Go through each of the _mesh's faces and retrieve the corresponding indices.
    uint32 tempNumFaces = pAMesh->mNumFaces;
    for (uint32 i = 0; i < tempNumFaces; ++i) {
      aiFace face = pAMesh->mFaces[i];

      //retrieve all indices of the face and store them in the indices vector
      uint32 tempNumIndices = face.mNumIndices;
      for (uint32 j = 0; j < tempNumIndices; ++j) {
        indices.push_back(face.mIndices[j]);
      }
    }

    //Walk through each of the _mesh's vertices.
    uint32 tempNumVertices = pAMesh->mNumVertices;
    for (uint32 i = 0; i < tempNumVertices; ++i) {
      //Positions
      vVertices[i].position.x = pAMesh->mVertices[i].x;
      vVertices[i].position.y = pAMesh->mVertices[i].y;
      vVertices[i].position.z = pAMesh->mVertices[i].z;

      //Tex coords
      //Check if _mesh contains tex coords
      if (pAMesh->mTextureCoords[0]) {
        vVertices[i].texCoords.x = pAMesh->mTextureCoords[0][i].x;
        vVertices[i].texCoords.y = pAMesh->mTextureCoords[0][i].y;
      }
      else {
        vVertices[i].texCoords = Vector2(0.0f, 0.0f);
      }

      if (0 != pAMesh->mNormals) {
        //Normals
        vVertices[i].normal.x = pAMesh->mNormals[i].x;
        vVertices[i].normal.y = pAMesh->mNormals[i].y;
        vVertices[i].normal.z = pAMesh->mNormals[i].z;
      }

      if (0 != pAMesh->mTangents) {
        //Tangents
        vVertices[i].tangent.x = pAMesh->mTangents[i].x;
        vVertices[i].tangent.y = pAMesh->mTangents[i].y;
        vVertices[i].tangent.z = pAMesh->mTangents[i].z;
      }

      if (0 != pAMesh->mBitangents) {
        //Bi tangents
        vVertices[i].tangent.x = pAMesh->mBitangents[i].x;
        vVertices[i].tangent.y = pAMesh->mBitangents[i].y;
        vVertices[i].tangent.z = pAMesh->mBitangents[i].z;
      }
    }

    newMesh = processBonesInfo(pAMesh, vVertices.data(), pAMesh->mNumVertices, newMesh);

    if (pAScene->HasMaterials()) {
      searchTextures(pAMesh, pAScene);
    }

    newMesh.setUpMesh(vVertices, indices, g_vTextures);
    g_vTextures.clear();
    return newMesh;
  }

  Mesh
  processBonesInfo(aiMesh* pAMesh, Vertex* vertexInfo, uint32 numVertexes, Mesh paramMesh) {
    SkeletalMesh* skeletal = new SkeletalMesh;

    if (0 < pAMesh->mNumBones) {
      uint32 tempNumBones = pAMesh->mNumBones;
      for (uint32 i = 0; i < tempNumBones; ++i) {
        auto bone = pAMesh->mBones[i];
        uint32 boneIndex = 0;
        String boneName(bone->mName.data);

        if (skeletal->bonesMap.find(boneName) == skeletal->bonesMap.end()) {
          boneIndex = skeletal->numBones;
          ++skeletal->numBones;

          Bone bi;
          skeletal->vBones.push_back(bi);
        }
        else {
          boneIndex = skeletal->bonesMap[boneName];
        }

        skeletal->bonesMap[boneName] = boneIndex;

        //Check if memcpy works
        memcpy(&skeletal->vBones[boneIndex].offSet, &bone->mOffsetMatrix, sizeof(Matrix4x4));

        uint32 tempNumWeights = bone->mNumWeights;
        for (uint32 j = 0; j < tempNumWeights; ++j) {
          uint32 verID = bone->mWeights[j].mVertexId;
          float weight = bone->mWeights[j].mWeight;

          for (uint32 k = 0; k < 4; ++k) {
            if (0 == vertexInfo[verID].boneWeights[k]) {
              vertexInfo[verID].boneIds[k] = boneIndex;
              vertexInfo[verID].boneWeights[k] = weight;
              break;
            }
          }
        }
      }
    }
    else {
      for (uint32 i = 0; i < numVertexes; ++i) {
        for (uint32 j = 0; j < 4; ++j) {
          vertexInfo[i].boneWeights[j] = 1;
        }
      }
    }

    SPtr<Vertex> SPtrVertexData(vertexInfo);

    paramMesh.m_skeletalMesh.reset(skeletal);
    paramMesh.setVertexData(SPtrVertexData);

    paramMesh.m_bonesTransforms.clear();
    paramMesh.m_bonesTransforms.resize(skeletal->numBones);

    return paramMesh;
  }

  void
  processAnimationInfo(const aiScene* pAScene) {
    ModelNodes* rootNode = new ModelNodes();

    g_animInfo.m_pStrModelNodes.reset(rootNode);

    loadModelNodes(g_animInfo.m_pStrModelNodes, pAScene->mRootNode);

    g_animInfo.setAnimName("FirstAnimation");

    uint32 tempNumAnims = pAScene->mNumAnimations;
    for (uint32 i = 0; i < tempNumAnims; ++i) {
      SPtr<AnimationData> newAnimation = make_shared<AnimationData>();

      newAnimation->m_animationName = pAScene->mAnimations[i]->mName.C_Str();
      g_animInfo.setAnimName(newAnimation->m_animationName);

      if (pAScene->mAnimations[i]->mTicksPerSecond != 0.0) {
        newAnimation->m_ticksPerSecond = (float)pAScene->mAnimations[i]->mTicksPerSecond;
      }
      else {
        newAnimation->m_ticksPerSecond = 25.0f;
      }

      newAnimation->m_animDuration = (float)pAScene->mAnimations[i]->mDuration;

      uint32 numCH;
      numCH = newAnimation->m_numChannels = pAScene->mAnimations[i]->mNumChannels;

      newAnimation->m_vChannels.resize(numCH);

      for (uint32 j = 0; j < numCH; ++j) {
        SPtr<AnimationNode> newAnimNode = make_shared<AnimationNode>();

        newAnimNode->m_nodeName =
          pAScene->mAnimations[i]->mChannels[j]->mNodeName.C_Str();

        newAnimNode->m_numPositionKeys =
          pAScene->mAnimations[i]->mChannels[j]->mNumPositionKeys;

        newAnimNode->m_vPositionKeys.resize(newAnimNode->m_numPositionKeys);

        uint32 tempNumPosKeys = newAnimNode->m_numPositionKeys;
        for (uint32 key = 0; key < tempNumPosKeys; ++key) {
          newAnimNode->m_vPositionKeys[key].m_time =
            pAScene->mAnimations[i]->mChannels[j]->mPositionKeys[key].mTime;

          newAnimNode->m_vPositionKeys[key].m_value.x =
            pAScene->mAnimations[i]->mChannels[j]->mPositionKeys[key].mValue.x;

          newAnimNode->m_vPositionKeys[key].m_value.y =
            pAScene->mAnimations[i]->mChannels[j]->mPositionKeys[key].mValue.y;

          newAnimNode->m_vPositionKeys[key].m_value.z =
            pAScene->mAnimations[i]->mChannels[j]->mPositionKeys[key].mValue.z;
        }

        newAnimNode->m_numRotationKeys =
          pAScene->mAnimations[i]->mChannels[j]->mNumRotationKeys;

        newAnimNode->m_vRotationKeys.resize(newAnimNode->m_numRotationKeys);

        uint32 tempNumRotKeys = newAnimNode->m_numRotationKeys;
        for (uint32 key = 0; key < tempNumRotKeys; ++key) {
          newAnimNode->m_vRotationKeys[key].m_time =
            pAScene->mAnimations[i]->mChannels[j]->mRotationKeys[key].mTime;

          newAnimNode->m_vRotationKeys[key].m_value.x =
            pAScene->mAnimations[i]->mChannels[j]->mRotationKeys[key].mValue.x;

          newAnimNode->m_vRotationKeys[key].m_value.y =
            pAScene->mAnimations[i]->mChannels[j]->mRotationKeys[key].mValue.y;

          newAnimNode->m_vRotationKeys[key].m_value.z =
            pAScene->mAnimations[i]->mChannels[j]->mRotationKeys[key].mValue.z;

          newAnimNode->m_vRotationKeys[key].m_value.w =
            pAScene->mAnimations[i]->mChannels[j]->mRotationKeys[key].mValue.w;
        }

        newAnimNode->m_numScalingKeys = pAScene->mAnimations[i]->mChannels[j]->mNumScalingKeys;
        newAnimNode->m_scalingKeys.resize(newAnimNode->m_numScalingKeys);

        uint32 tempNumScalKeys = newAnimNode->m_numScalingKeys;
        for (uint32 key = 0; key < tempNumScalKeys; ++key) {
          newAnimNode->m_scalingKeys[key].m_time =
            pAScene->mAnimations[i]->mChannels[j]->mScalingKeys[key].mTime;

          newAnimNode->m_scalingKeys[key].m_value.x =
            pAScene->mAnimations[i]->mChannels[j]->mScalingKeys[key].mValue.x;

          newAnimNode->m_scalingKeys[key].m_value.y =
            pAScene->mAnimations[i]->mChannels[j]->mScalingKeys[key].mValue.y;

          newAnimNode->m_scalingKeys[key].m_value.z =
            pAScene->mAnimations[i]->mChannels[j]->mScalingKeys[key].mValue.z;
        }

        newAnimation->m_vChannels[j] = newAnimNode;
      }

      g_vAnimationData.push_back(newAnimation);
    }

    if (0 < pAScene->mNumAnimations) {
      g_animInfo.setNumAnims((uint32)pAScene->mNumAnimations);
    }
  }

  void
  loadModelNodes(WeakSPtr<ModelNodes> myNode, const aiNode* ainode) {
    SPtr<ModelNodes> modelNode = make_shared<ModelNodes>();
    modelNode = myNode.lock();

    uint32 num;

    modelNode->m_name = ainode->mName.C_Str();
    num = modelNode->m_numChildrens = (uint32)ainode->mNumChildren;
    modelNode->m_numMeshes = (uint32)ainode->mNumMeshes;

    memcpy(&modelNode->m_transformMatrix, &ainode->mTransformation, sizeof(Matrix4x4));

    modelNode->m_vChildrenNodes.resize(num);

    for (uint32 i = 0; i < num; ++i) {
      ModelNodes* newNode = new ModelNodes;
      newNode->m_parentNode = modelNode;
      modelNode->m_vChildrenNodes[i].reset(newNode);
      loadModelNodes(modelNode->m_vChildrenNodes[i], ainode->mChildren[i]);
    }
  }

  String obtainTexturePath(String file) {
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

  String deleteDobleSlash(String file) {
    uint32 tempSize = file.size();
    String tempStr;
    for (uint32 i = 0; i < tempSize; ++i) {
      tempStr += file[i];

      if ('\\' == tempStr[i]) {
        uint32 tempSlashPos = tempStr.rfind('\\');
        tempStr.erase(tempSlashPos, 1);
      }
      else if ('//' == tempStr[i]) {
        int32 tempSlashPos = tempStr.rfind('//');
        tempStr.erase(tempSlashPos, 1);
      }
    }

    return tempStr;
  }

  void 
  searchTextures(aiMesh* pAMesh, const aiScene* pAScene) {
    auto& matIndex = pAMesh->mMaterialIndex;
    auto& meshMaterial = pAScene->mMaterials[matIndex];
    auto& meshName = pAMesh->mName;
    g_vMeshName.push_back(meshName.C_Str());

    aiString tempMatName;
    meshMaterial->Get(AI_MATKEY_NAME, tempMatName);
    
    String matId(tempMatName.C_Str());

    if (g_mMaterials.end() != g_mMaterials.find(matId)) {
      //Found the material
    }
    else {
      g_mMaterials[matId] = make_shared<Materials>();
      SPtr<Materials> newMaterial = g_mMaterials[matId];

      aiString textureNames[6];
      meshMaterial->Get(AI_MATKEY_TEXTURE_DIFFUSE(0),   textureNames[0]);
      meshMaterial->Get(AI_MATKEY_TEXTURE_SPECULAR(0),  textureNames[1]);
      meshMaterial->Get(AI_MATKEY_TEXTURE_SHININESS(0), textureNames[2]);
      meshMaterial->Get(AI_MATKEY_TEXTURE_NORMALS(0),   textureNames[3]);
      meshMaterial->Get(AI_MATKEY_TEXTURE_EMISSIVE(0),  textureNames[4]);
      meshMaterial->Get(AI_MATKEY_TEXTURE_OPACITY(0),   textureNames[5]);

      //Get the file path without useless information.
      String srcFile[6];
      for (uint32 i = 0; i < 6; ++i) {
        if (0 != textureNames[i].length) {
          srcFile[i] = String(textureNames[i].C_Str());
          srcFile[i] = g_texturesDirectory + obtainTexturePath(srcFile[i]);
          srcFile[i] = deleteDobleSlash(srcFile[i]);
        }
      }

      String texName(tempMatName.C_Str());

      //Base Color
      if ("" != srcFile[0]) {
        newMaterial->setTexture("Base Color", srcFile[0], texName);
      }
      else {
        newMaterial->m_vTextures.push_back(nullptr);
      }
      //Metallic
      if ("" != srcFile[1]) {
        newMaterial->setTexture("Metallic", srcFile[1], texName);
      }
      else {
        newMaterial->m_vTextures.push_back(nullptr);
      }
      //Roughness
      if ("" != srcFile[2]) {
        newMaterial->setTexture("Roughness", srcFile[2], texName);
      }
      else {
        newMaterial->m_vTextures.push_back(nullptr);
      }
      //Normals
      if ("" != srcFile[3]) {
        newMaterial->setTexture("Normals", srcFile[3], texName);
      }
      else {
        newMaterial->m_vTextures.push_back(nullptr);
      }
      //Emissive
      if ("" != srcFile[4]) {
        newMaterial->setTexture("Emissive", srcFile[4], texName);
      }
      else {
        newMaterial->m_vTextures.push_back(nullptr);
      }
      //Opacity
      if ("" != srcFile[5]) {
        newMaterial->setTexture("Opacity", srcFile[5], texName);
      }
      else {
        newMaterial->m_vTextures.push_back(nullptr);
      }

      g_mMaterials.insert(pair<String, SPtr<Materials>>(texName, newMaterial));

      if (0 != newMaterial->m_vTextures.size()) {
        for (uint32 i = 0; i < 6; ++i) {
          g_vTextures.push_back(newMaterial->m_vTextures[i]);
        }
      }
    }
  }

  /***************************************************************************/
  /**
  * Protected.
  */
  /***************************************************************************/

  void 
  Models::createDirectories(const String& file) {
    //Retrieve the directory file of the _file
    m_modelDirectory = file.substr(0, file.find_last_of('/'));

    //This is to get the file of the textures.
    String miniPaht;

    uint32 tempModelDirecSize = m_modelDirectory.size();
    for (uint32 i = 0; i < tempModelDirecSize; ++i) {
      miniPaht += m_modelDirectory.at(i);
      if ("data/models/" == miniPaht) {
        g_texturesDirectory = miniPaht;
        g_texturesDirectory.replace(5, 9, "textures/");
        miniPaht.clear();
      }
    }
    g_texturesDirectory += miniPaht + '/';
    miniPaht.clear();
  }
}