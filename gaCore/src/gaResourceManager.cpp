#include "gaResourceManager.h"
#include "gaGraphicsApi.h"
#include "gaMesh.h"

namespace gaEngineSDK {
  void 
  ResourceManager::initLoadModel(String const& path) {
    //Read _file via assimp
    m_pAScene = m_aImporter.ReadFile(path,
                                     aiProcessPreset_TargetRealtime_MaxQuality |
                                     aiProcess_ConvertToLeftHanded |
                                     aiProcess_Triangulate);
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

    processAnimationInfo();
  }

  void
  ResourceManager::processNode(aiNode* pANode, const aiScene* pAScene) {
    //Process each _mesh located at the current pNode
    uint32 tempNumMeshes = pANode->mNumMeshes;
    for (uint32 i = 0; i < tempNumMeshes; ++i) {
      /*
      * The pNode object only contains indices to index the actual objects in the pAScene.
      * The pAScene contains all the data, pNode is just to keep stuff organized.
      */
      aiMesh* _mesh = pAScene->mMeshes[pANode->mMeshes[i]];
      m_vMeshes.push_back(processMesh(_mesh, pAScene));
    }

    uint32 tempNumChildren = pANode->mNumChildren;
    for (uint32 i = 0; i < tempNumChildren; ++i) {
      processNode(pANode->mChildren[i], pAScene);
    }
  }

  SPtr<Mesh>
  ResourceManager::processMesh(aiMesh* pAMesh, const aiScene* pAScene) {
    //Data to fill
    Vector<uint32> indices;
    Vector<Vertex> vertices;
    Vector<Texture> textures;
    uint32 numVertex = pAMesh->mNumVertices;
    Vertex* structVertex = new Vertex[numVertex];
    m_newMesh = new Mesh();

    if (!pAMesh->HasFaces()) {
      throw new std::exception("Error, no se detectaron caras");
    }

    //Go through each of the _mesh's faces and retrieve the corresponding indices.
    uint32 tempNumFaces = pAMesh->mNumFaces;
    for (uint32 i = 0; i < tempNumFaces; ++i) {
      aiFace face = pAMesh->mFaces[i];

      //retrieve all indices of the face and store them in the indices vector
      uint32 tempNumIndices = face.mNumIndices;
      for (uint32 i = 0; i < tempNumIndices; ++i) {
        indices.push_back(face.mIndices[i]);
      }
    }

    //Walk through each of the _mesh's vertices.
    uint32 tempNumVertices = pAMesh->mNumVertices;
    for (uint32 i = 0; i < tempNumVertices; ++i) {
      //Positions
      structVertex[i].position.x = pAMesh->mVertices[i].x;
      structVertex[i].position.y = pAMesh->mVertices[i].y;
      structVertex[i].position.z = pAMesh->mVertices[i].z;

      //Texcoords
      //Check if _mesh contains texcoords
      if (pAMesh->mTextureCoords[0]) {
        structVertex[i].texCoords.x = pAMesh->mTextureCoords[0][i].x;
        structVertex[i].texCoords.y = pAMesh->mTextureCoords[0][i].y;
      }
      else {
        structVertex[i].texCoords = Vector2(0.0f, 0.0f);
      }

      if (0 != pAMesh->mNormals) {
        //Normals
        structVertex[i].normal.x = pAMesh->mNormals[i].x;
        structVertex[i].normal.y = pAMesh->mNormals[i].y;
        structVertex[i].normal.z = pAMesh->mNormals[i].z;
      }

      if (0 != pAMesh->mTangents) {
        //Tangents
        structVertex[i].tangent.x = pAMesh->mTangents[i].x;
        structVertex[i].tangent.y = pAMesh->mTangents[i].y;
        structVertex[i].tangent.z = pAMesh->mTangents[i].z;
      }

      if (0 != pAMesh->mBitangents) {
        //Bitangents
        structVertex[i].tangent.x = pAMesh->mBitangents[i].x;
        structVertex[i].tangent.y = pAMesh->mBitangents[i].y;
        structVertex[i].tangent.z = pAMesh->mBitangents[i].z;
      }

      vertices.push_back(structVertex[i]);
    }

    processBonesInfo(pAMesh, vertices.data(), numVertex);

    //processIndexInfo((uint32)indices.size());

    aiMaterial* material = pAScene->mMaterials[pAMesh->mMaterialIndex];

    Vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE);

    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

    m_newMesh->init(vertices, indices, textures);

    auto myGraphicApi = g_graphicApi().instancePtr();

    SPtr<Mesh> tempMesh(m_newMesh);
    return tempMesh;
  }

  void 
  ResourceManager::processBonesInfo(aiMesh* pAMesh, Vertex* structureVertex, 
                                    uint32 numVertexes) {
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

        //Checar si funciona el memcopy
        std::memcpy(&skeletal->vBones[boneIndex].offSet, &pAMesh->mBones[i]->mOffsetMatrix,
                    sizeof(Matrix4x4));

        uint32 tempNumWeights = pAMesh->mBones[i]->mNumWeights;
        for (uint32 j = 0; j < tempNumWeights; ++j) {
          uint32 verID = pAMesh->mBones[i]->mWeights[j].mVertexId;
          float weight = pAMesh->mBones[i]->mWeights[j].mWeight;

          for (uint32 k = 0; k < 4; ++k) {
            if (0 == structureVertex[verID].boneWeights[k]) {
              structureVertex[verID].boneIds[k] = boneIndex;
              structureVertex[verID].boneWeights[k] = weight;
              break;
            }
          }
        }
      }
    }
    else {
      for (uint32 i = 0; i < numVertexes; ++i) {
        for (uint32 j = 0; j < 4; ++j) {
          structureVertex[i].boneWeights[j] = 1;
        }
      }
    }

    SPtr<Vertex> SPtrVertexData(structureVertex);

    m_newMesh->m_skeletalMesh.reset(skeletal);
    m_newMesh->setVertexData(SPtrVertexData);

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

    m_newModel->m_globalInverseTransform.invert(m_newModel->m_globalInverseTransform);

    ModelNodes* rootNode = new ModelNodes();

    m_newModel->m_modelNodes.reset(rootNode);

    loadModelNodes(m_newModel->m_modelNodes, m_pAScene->mRootNode);

    m_newModel->m_pAnimationsList.push_back((char*)"None");

    uint32 tempNumAnims = m_pAScene->mNumAnimations;
    for (uint32 i = 0; i < tempNumAnims; ++i) {
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

        uint32 tempNumPosKeys = newAnimNode->m_numPositionKeys;
        for (uint32 key = 0; key < tempNumPosKeys; ++key) {
          newAnimNode->m_vPositionKeys[key].m_time = 
                       m_pAScene->mAnimations[i]->mChannels[j]->mPositionKeys[key].mTime;

          newAnimNode->m_vPositionKeys[key].m_value.x =
                       m_pAScene->mAnimations[i]->mChannels[j]->mPositionKeys[key].mValue.x;

          newAnimNode->m_vPositionKeys[key].m_value.y =
                       m_pAScene->mAnimations[i]->mChannels[j]->mPositionKeys[key].mValue.y;

          newAnimNode->m_vPositionKeys[key].m_value.z =
                       m_pAScene->mAnimations[i]->mChannels[j]->mPositionKeys[key].mValue.z;
        }

        newAnimNode->m_numRotationKeys = 
                     m_pAScene->mAnimations[i]->mChannels[j]->mNumRotationKeys;

        newAnimNode->m_vRotationKeys.resize(newAnimNode->m_numRotationKeys);

        uint32 tempNumRotKeys = newAnimNode->m_numRotationKeys;
        for (uint32 key = 0; key < tempNumRotKeys; ++key) {
          newAnimNode->m_vRotationKeys[key].m_time =
                       m_pAScene->mAnimations[i]->mChannels[j]->mRotationKeys[key].mTime;

          newAnimNode->m_vRotationKeys[key].m_value.x =
                       m_pAScene->mAnimations[i]->mChannels[j]->mRotationKeys[key].mValue.x;

          newAnimNode->m_vRotationKeys[key].m_value.y =
                       m_pAScene->mAnimations[i]->mChannels[j]->mRotationKeys[key].mValue.y;

          newAnimNode->m_vRotationKeys[key].m_value.z =
                       m_pAScene->mAnimations[i]->mChannels[j]->mRotationKeys[key].mValue.z;

          newAnimNode->m_vRotationKeys[key].m_value.w =
                       m_pAScene->mAnimations[i]->mChannels[j]->mRotationKeys[key].mValue.w;
        }

        newAnimNode->m_numScalingKeys = m_pAScene->mAnimations[i]->mChannels[j]->mNumScalingKeys;
        newAnimNode->m_scalingKeys.resize(newAnimNode->m_numScalingKeys);

        uint32 tempNumScalKeys = newAnimNode->m_numScalingKeys;
        for (uint32 key = 0; key < tempNumScalKeys; ++key) {
          newAnimNode->m_scalingKeys[key].m_time =
                       m_pAScene->mAnimations[i]->mChannels[j]->mScalingKeys[key].mTime;

          newAnimNode->m_scalingKeys[key].m_value.x =
                       m_pAScene->mAnimations[i]->mChannels[j]->mScalingKeys[key].mValue.x;

          newAnimNode->m_scalingKeys[key].m_value.y =
                       m_pAScene->mAnimations[i]->mChannels[j]->mScalingKeys[key].mValue.y;

          newAnimNode->m_scalingKeys[key].m_value.z =
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

  Vector<Texture> 
  ResourceManager::loadMaterialTextures(aiMaterial* pAMat, aiTextureType Atype) {
    Vector<Texture> textures;

    uint32 tempTypeTexture = pAMat->GetTextureCount(Atype);
    for (uint32 i = 0; i < tempTypeTexture; ++i) {
      aiString aistr;

      pAMat->GetTexture(Atype, i, &aistr);

      String srcFile = String(aistr.C_Str());
      srcFile = m_texturesDirectory + '/' + getTexturePath(srcFile);

      bool skip = false;

      uint32 tempTextureSize = m_textures.size();
      for (uint32 i = 0; i < tempTextureSize; ++i) {
        if (std::strcmp(m_textures[i].path.data(), srcFile.data()) == 0) {
          textures.push_back(m_textures[i]);
          skip = true;
          break;
        }
      }
      if (!skip) {
        auto myGraphicApi = g_graphicApi().instancePtr();

        Texture meshTexture;
        meshTexture.texture.reset(myGraphicApi->loadTextureFromFile(srcFile));
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

  Vector<SPtr<Mesh>>
  ResourceManager::getMeshes() {
    return m_vMeshes;
  }

  Vector<SamplerState*>
  ResourceManager::getSamplerInfo() {
    return m_newModel->getSamplerInfo();
  }

  Vector<SPtr<Model>>
  ResourceManager::getVecModels() {
    return m_vModels;
  }

  Mesh* 
  ResourceManager::getMesh() {
    return m_newMesh;
  }

  SPtr<Model> 
  ResourceManager::getModel() {
    return m_newModel;
  }

  void
  ResourceManager::createDirectories(String const& path) {
    //Retrieve the directory path of the _file
    m_modelDirectory = path.substr(0, path.find_last_of('/'));

    //Esto es para obtener el path de las texturas.
    String miniPaht;

    uint32 tempModelDirecSize = m_modelDirectory.size();
    for (uint32 i = 0; i < tempModelDirecSize; ++i) {
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