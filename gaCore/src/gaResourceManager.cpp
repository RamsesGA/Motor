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
    m_newModel.reset(new Model());

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
    //DVertex::E structVertex;
    uint32 numVertex = pAMesh->mNumVertices;
    Vertex::E* structVertex = new Vertex::E[numVertex];

    if (!pAMesh->HasFaces()) {
      throw new std::exception("Error, no se detectaron caras");
    }

    //Go through each of the _mesh's faces and retrieve the corresponding indices.
    for (uint32 i = 0; i < pAMesh->mNumFaces; i++) {
      aiFace face = pAMesh->mFaces[i];

      //retrieve all indices of the face and store them in the indices vector
      for (uint32 i = 0; i < face.mNumIndices; i++) {
        indices.push_back(face.mIndices[i]);
      }
    }

    //Walk through each of the _mesh's vertices.
    for (uint32 i = 0; i < pAMesh->mNumVertices; i++) {
      //Positions
      structVertex[i].position.m_x = pAMesh->mVertices[i].x;
      structVertex[i].position.m_y = pAMesh->mVertices[i].y;
      structVertex[i].position.m_z = pAMesh->mVertices[i].z;

      //Texcoords
      //Check if _mesh contains texcoords
      if (pAMesh->mTextureCoords[0]) {
        structVertex[i].texCoords.m_x = pAMesh->mTextureCoords[0][i].x;
        structVertex[i].texCoords.m_y = pAMesh->mTextureCoords[0][i].y;
      }
      else {
        structVertex[i].texCoords = Vector2(0.0f, 0.0f);
      }

      //Normals
      structVertex[i].normal.m_x = pAMesh->mNormals[i].x;
      structVertex[i].normal.m_y = pAMesh->mNormals[i].y;
      structVertex[i].normal.m_z = pAMesh->mNormals[i].z;

      //Tangents
      structVertex[i].tangent.m_x = pAMesh->mTangents[i].x;
      structVertex[i].tangent.m_y = pAMesh->mTangents[i].y;
      structVertex[i].tangent.m_z = pAMesh->mTangents[i].z;

      //Bitangents
      structVertex[i].tangent.m_x = pAMesh->mBitangents[i].x;
      structVertex[i].tangent.m_y = pAMesh->mBitangents[i].y;
      structVertex[i].tangent.m_z = pAMesh->mBitangents[i].z;

      vertices.push_back(structVertex[i]);
    }

    m_newMesh = new Mesh();
    processBonesInfo(pAMesh, structVertex, numVertex);

    //processIndexInfo((uint32)indices.size());

    processAnimationInfo();

    aiMaterial* material = pAScene->mMaterials[pAMesh->mMaterialIndex];

    Vector<Texture::E> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE);

    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

    m_newMesh->init(vertices, indices, textures);

    return m_newMesh;
  }

  void 
  ResourceManager::processBonesInfo(aiMesh* pAMesh, Vertex::E* structureVertex, 
                                    uint32 numVertexes) {
    SkeletalMesh* skeletal = new SkeletalMesh;
    
    if (0 < pAMesh->mNumBones) {
      for (uint32 i = 0; i < pAMesh->mNumBones; ++i) {
        auto bone = pAMesh->mBones[i];

        uint32 boneIndex = 0;
        String boneName(bone->mName.data);

        if (skeletal->bonesMap.find(boneName) == skeletal->bonesMap.end()) {
          boneIndex = skeletal->numBones;
          skeletal->numBones++;

          Bone bi;
          skeletal->vBones.push_back(bi);
        }
        else {
          boneIndex = skeletal->bonesMap[boneName];
        }

        skeletal->bonesMap[boneName] = boneIndex;

        auto uno = sizeof(skeletal->vBones[boneIndex].offSet);
        auto dos = sizeof(pAMesh->mBones[i]->mOffsetMatrix);
        auto tres = sizeof(Matrix4x4);

        //Checar si funciona el memcopy
        //std::memcpy(&skeletal->vBones[boneIndex].offSet, &pAMesh->mBones[i]->mOffsetMatrix,
        //            sizeof(Matrix4x4));

        for (uint32 j = 0; j < pAMesh->mBones[i]->mNumWeights; ++j) {
          uint32 verID = pAMesh->mBones[i]->mWeights[j].mVertexId;

          float weight = pAMesh->mBones[i]->mWeights[j].mWeight;

          for (uint32 k = 0; k < 4; ++k) {
            if (structureVertex[verID].boneWeights[k] == 0) {
              structureVertex[verID].boneIds[k] = boneIndex;
              structureVertex[verID].boneWeights[k] = weight;
              break;
            }
          }
        }
      }
    }
    else {
      for (uint32 i = 0; i < numVertexes; i++) {
        for (uint32 j = 0; j < 4; ++j) {
          structureVertex[i].boneWeights[j] = 1;
        }
      }
    }

    SPtr<Vertex::E> SPtrVertexData(structureVertex);

    m_newMesh->m_skeletalMesh.reset(skeletal);
    m_newMesh->setVertexData(SPtrVertexData);

    auto myGraphicApi = g_graphicApi().instancePtr();

    m_newMesh->setVertexBuffer(myGraphicApi->createVertexBuffer(structureVertex,
                                                              sizeof(Vertex::E)));
    m_newMesh->m_bonesTransforms.clear();
    m_newMesh->m_bonesTransforms.resize(skeletal->numBones);
  }

  void 
  ResourceManager::processIndexInfo(uint32 numIndices) {
    //TODO
  }

  void 
  ResourceManager::processAnimationInfo() {
    std::memcpy(&m_newModel->m_globalInverseTransform, &m_pAScene->mRootNode->mTransformation,
                sizeof(Matrix4x4));

    m_newModel->m_globalInverseTransform.transpose();
    ModelNodes* rootNode = new ModelNodes();

    m_newModel->m_modelNodes.reset(rootNode);

    loadModelNodes(m_newModel->m_modelNodes, m_pAScene->mRootNode);

    m_newModel->m_pAnimationsList.push_back((char*)"None");

    for (uint32 i = 0; i < m_pAScene->mNumAnimations; ++i) {
      AnimationData* newAnimation = new AnimationData;

      newAnimation->m_animationName = m_pAScene->mAnimations[i]->mName.C_Str();
      m_newModel->m_pAnimationsList.push_back((char*)newAnimation->m_animationName.c_str());

      if (m_pAScene->mAnimations[i]->mTicksPerSecond != 0.0) {
        newAnimation->m_ticksPerSecond = (float)m_pAScene->mAnimations[i]->mTicksPerSecond;
      }
      else {
        newAnimation->m_ticksPerSecond = 25.0f;
      }

      newAnimation->m_animDuration = (float)m_pAScene->mAnimations[i]->mDuration;
      uint32 numCH;

      numCH = newAnimation->m_numChannels = m_pAScene->mAnimations[i]->mNumChannels;
      newAnimation->m_vChannels.resize(numCH);

      for (uint32 j = 0; j < numCH; ++j) {
        SPtr<AnimationNode> newAnimNode(new AnimationNode);
        newAnimNode->m_nodeName = 
                     m_pAScene->mAnimations[i]->mChannels[j]->mNodeName.C_Str();

        newAnimNode->m_numPositionKeys = 
                     m_pAScene->mAnimations[i]->mChannels[j]->mNumPositionKeys;

        newAnimNode->m_vPositionKeys.resize(newAnimNode->m_numPositionKeys);

        for (uint32 key = 0; key < newAnimNode->m_numPositionKeys; ++key) {
          newAnimNode->m_vPositionKeys[key].m_time = 
                       m_pAScene->mAnimations[i]->mChannels[j]->mPositionKeys[key].mTime;

          newAnimNode->m_vPositionKeys[key].m_value.m_x =
                       m_pAScene->mAnimations[i]->mChannels[j]->mPositionKeys[key].mValue.x;

          newAnimNode->m_vPositionKeys[key].m_value.m_y =
                       m_pAScene->mAnimations[i]->mChannels[j]->mPositionKeys[key].mValue.y;

          newAnimNode->m_vPositionKeys[key].m_value.m_z =
                       m_pAScene->mAnimations[i]->mChannels[j]->mPositionKeys[key].mValue.z;
        }

        newAnimNode->m_numRotationKeys = 
                     m_pAScene->mAnimations[i]->mChannels[j]->mNumRotationKeys;

        newAnimNode->m_vRotationKeys.resize(newAnimNode->m_numRotationKeys);

        for (uint32 key = 0; key < newAnimNode->m_numRotationKeys; ++key) {
          newAnimNode->m_vRotationKeys[key].m_time =
                       m_pAScene->mAnimations[i]->mChannels[j]->mRotationKeys[key].mTime;

          newAnimNode->m_vRotationKeys[key].m_value.m_x =
                       m_pAScene->mAnimations[i]->mChannels[j]->mRotationKeys[key].mValue.x;

          newAnimNode->m_vRotationKeys[key].m_value.m_y =
                       m_pAScene->mAnimations[i]->mChannels[j]->mRotationKeys[key].mValue.y;

          newAnimNode->m_vRotationKeys[key].m_value.m_z =
                       m_pAScene->mAnimations[i]->mChannels[j]->mRotationKeys[key].mValue.z;

          newAnimNode->m_vRotationKeys[key].m_value.m_w =
                       m_pAScene->mAnimations[i]->mChannels[j]->mRotationKeys[key].mValue.w;
        }

        newAnimNode->m_numScalingKeys = m_pAScene->mAnimations[i]->mChannels[j]->mNumScalingKeys;
        newAnimNode->m_scalingKeys.resize(newAnimNode->m_numScalingKeys);

        for (uint32 key = 0; key < newAnimNode->m_numScalingKeys; ++key) {
          newAnimNode->m_scalingKeys[key].m_time =
                       m_pAScene->mAnimations[i]->mChannels[j]->mScalingKeys[key].mTime;

          newAnimNode->m_scalingKeys[key].m_value.m_x =
                       m_pAScene->mAnimations[i]->mChannels[j]->mScalingKeys[key].mValue.x;

          newAnimNode->m_scalingKeys[key].m_value.m_y =
                       m_pAScene->mAnimations[i]->mChannels[j]->mScalingKeys[key].mValue.y;

          newAnimNode->m_scalingKeys[key].m_value.m_z =
                       m_pAScene->mAnimations[i]->mChannels[j]->mScalingKeys[key].mValue.z;
        }

        newAnimation->m_vChannels[j] = newAnimNode;
      }
      SPtr<AnimationData> anim(newAnimation);
      m_newModel->m_vAnimationData.push_back(anim);
    }

    if (0 < m_pAScene->mNumAnimations) {
      m_newModel->m_numAnimations = (uint32)m_pAScene->mNumAnimations;
    }

    SPtr<Model> modelCreate(m_newModel);

    m_vModels.push_back(modelCreate);
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

      for (uint32 i = 0; i < m_textures.size(); i++) {
        if (std::strcmp(m_textures[i].path.data(), srcFile.data()) == 0) {
          textures.push_back(m_textures[i]);
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

  void 
  ResourceManager::loadModelNodes(WeakSPtr<ModelNodes> myNode, const aiNode* ainode) {
    SPtr<ModelNodes> modelNode = myNode.lock();
    uint32 num;

    modelNode->m_name = ainode->mName.C_Str();
    num = modelNode->m_numChildrens = (uint32)ainode->mNumChildren;
    modelNode->m_numMeshes = (uint32)ainode->mNumMeshes;

    std::memcpy(&modelNode->m_transformMatrix, &ainode->mTransformation, sizeof(Matrix4x4));

    modelNode->m_vChildrenNodes.resize(num);

    for (uint32 i = 0; i < num; ++i) {
      ModelNodes* newNode = new ModelNodes;
      newNode->m_parentNode = modelNode;
      modelNode->m_vChildrenNodes[i].reset(newNode);
      loadModelNodes(modelNode->m_vChildrenNodes[i], ainode->mChildren[i]);
    }
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

  Vector<SamplerState*>

    ResourceManager::getSamplerInfo() {
    return m_newModel->getSamplerInfo();
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