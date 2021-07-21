#include "gaResourceManager.h"
#include "gaGraphicsApi.h"

namespace gaEngineSDK {
  const aiScene* g_pAScene = nullptr;

  ResourceManager::ResourceManager(const String& path) {
    Assimp::Importer aImporter;

    //Read _file via assimp
    g_pAScene = aImporter.ReadFile(path, aiProcessPreset_TargetRealtime_MaxQuality |
                                   aiProcess_ConvertToLeftHanded | aiProcess_Triangulate);

    //Check for errors
    if (!g_pAScene || g_pAScene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || 
        !g_pAScene->mRootNode) {
      return;
    }

    //We create the directory of the model and its texture
    createDirectories(path);

    auto myGraphicApi = g_graphicApi().instancePtr();
    m_pAnimation.reset(new Animations());

    //Process assimp's root pNode recursively
    processNode(g_pAScene->mRootNode);

    processAnimationInfo();

    //SPtr<SamplerState> tempSamp(myGraphicApi->createSamplerState());

    //Create texture sampler for model's textures
    //m_pAnimation->setSampler(tempSamp);

    m_meshBones.resize(m_vMeshes.size());
  }

  /***************************************************************************/
  /**
  * Inheritance methods.
  */
  /***************************************************************************/

  void
  ResourceManager::update(const float& deltaTime) {
    if (0 != m_vAnimationData.size()) {
      SPtr<AnimationData> currentAnimation = m_vAnimationData[0];

      if (m_playAnimation) {
        m_timeOfAnimation += deltaTime;
      }
      if (m_timeOfAnimation >= currentAnimation->m_animDuration) {
        m_timeOfAnimation = 0.0f;
      }

      uint32 meshNum = 0;

      for (auto mesh : m_vMeshes) {
        mesh->m_cbBonesTrans = &m_meshBones[meshNum];
        mesh->animated(m_timeOfAnimation, currentAnimation, m_pAnimation->m_pStrModelNodes);
        ++meshNum;
      }
    }
  }

  void 
  ResourceManager::render() {
    auto myGraphicApi = g_graphicApi().instancePtr();

    uint32 meshNum = 0;

    for (auto mesh : m_vMeshes) {
      myGraphicApi->updateConstantBuffer(&m_meshBones[meshNum], 
                                         myGraphicApi->getConstBufferBones());

      if (0 != m_vMeshes[meshNum]->m_textures.size()) {

        //TODO: I need to correct the sampler
        myGraphicApi->setShaderResourceView(m_vMeshes[meshNum]->m_textures, meshNum, 1);
      }

      myGraphicApi->setVertexBuffer(mesh->m_pVertexBuffer);
      myGraphicApi->setIndexBuffer(mesh->m_pIndexBuffer);

      myGraphicApi->drawIndex(mesh->getNumIndices());
      ++meshNum;
    }
  }

  /***************************************************************************/
  /**
  * Methods.
  */
  /***************************************************************************/

  void
  ResourceManager::processNode(aiNode* pANode) {
    //Process each _mesh located at the current pNode
    uint32 tempNumMeshes = pANode->mNumMeshes;

    for (uint32 i = 0; i < tempNumMeshes; ++i) {
      /*
      * The pNode object only contains indices to index the actual objects in the g_pAScene.
      * The g_pAScene contains all the data, pNode is just to keep stuff organized.
      */
      aiMesh* _mesh = g_pAScene->mMeshes[pANode->mMeshes[i]];
      m_vMeshes.push_back(processMesh(_mesh));
    }

    uint32 tempNumChildren = pANode->mNumChildren;
    for (uint32 i = 0; i < tempNumChildren; ++i) {
      processNode(pANode->mChildren[i]);
    }
  }

  SPtr<Mesh>
  ResourceManager::processMesh(aiMesh* pAMesh) {
    //Data to fill
    Vector<uint32> indices;
    Vector<Textures*> textures;
    Vector<Vertex> vVertices;

    vVertices.resize(pAMesh->mNumVertices);

    m_newMesh = new Mesh();

    if (!pAMesh->HasFaces()) {
      return nullptr;
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

    processBonesInfo(pAMesh, vVertices.data(), pAMesh->mNumVertices);




    aiMaterial* material = g_pAScene->mMaterials[pAMesh->mMaterialIndex];

    Vector<Textures*> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE);

    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

    m_newMesh->init(vVertices, indices, textures);

    auto myGraphicApi = g_graphicApi().instancePtr();

    SPtr<Mesh> tempMesh(m_newMesh);
    return tempMesh;
  }

  void 
  ResourceManager::processBonesInfo(aiMesh* pAMesh, Vertex* vertexInfo, uint32 numVertexes) {
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
        std::memcpy(&skeletal->vBones[boneIndex].offSet, &bone->mOffsetMatrix,
                    sizeof(Matrix4x4));

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
    //std::memcpy(&m_pAnimation->m_globalInverseTransform, &g_pAScene->mRootNode->mTransformation, sizeof(Matrix4x4));
    //m_pAnimation->m_globalInverseTransform.transpose();
    //m_pAnimation->m_globalInverseTransform.invert(m_pAnimation->m_globalInverseTransform);

    ModelNodes* rootNode = new ModelNodes();

    m_pAnimation->m_pStrModelNodes.reset(rootNode);

    loadModelNodes(m_pAnimation->m_pStrModelNodes, g_pAScene->mRootNode);

    m_pAnimation->setAnimName("FirstAnimation");

    uint32 tempNumAnims = g_pAScene->mNumAnimations;
    for (uint32 i = 0; i < tempNumAnims; ++i) {
      AnimationData* newAnimation = new AnimationData;

      newAnimation->m_animationName = g_pAScene->mAnimations[i]->mName.C_Str();
      m_pAnimation->setAnimName(newAnimation->m_animationName);

      if (g_pAScene->mAnimations[i]->mTicksPerSecond != 0.0) {
        newAnimation->m_ticksPerSecond = (float)g_pAScene->mAnimations[i]->mTicksPerSecond;
      }
      else {
        newAnimation->m_ticksPerSecond = 25.0f;
      }

      newAnimation->m_animDuration = (float)g_pAScene->mAnimations[i]->mDuration;
      
      uint32 numCH;
      numCH = newAnimation->m_numChannels = g_pAScene->mAnimations[i]->mNumChannels;
      
      newAnimation->m_vChannels.resize(numCH);

      for (uint32 j = 0; j < numCH; ++j) {
        SPtr<AnimationNode> newAnimNode(new AnimationNode);

        newAnimNode->m_nodeName = 
                     g_pAScene->mAnimations[i]->mChannels[j]->mNodeName.C_Str();

        newAnimNode->m_numPositionKeys = 
                     g_pAScene->mAnimations[i]->mChannels[j]->mNumPositionKeys;

        newAnimNode->m_vPositionKeys.resize(newAnimNode->m_numPositionKeys);

        uint32 tempNumPosKeys = newAnimNode->m_numPositionKeys;
        for (uint32 key = 0; key < tempNumPosKeys; ++key) {
          newAnimNode->m_vPositionKeys[key].m_time = 
                       g_pAScene->mAnimations[i]->mChannels[j]->mPositionKeys[key].mTime;

          newAnimNode->m_vPositionKeys[key].m_value.x =
                       g_pAScene->mAnimations[i]->mChannels[j]->mPositionKeys[key].mValue.x;

          newAnimNode->m_vPositionKeys[key].m_value.y =
                       g_pAScene->mAnimations[i]->mChannels[j]->mPositionKeys[key].mValue.y;

          newAnimNode->m_vPositionKeys[key].m_value.z =
                       g_pAScene->mAnimations[i]->mChannels[j]->mPositionKeys[key].mValue.z;
        }

        newAnimNode->m_numRotationKeys = 
                     g_pAScene->mAnimations[i]->mChannels[j]->mNumRotationKeys;

        newAnimNode->m_vRotationKeys.resize(newAnimNode->m_numRotationKeys);

        uint32 tempNumRotKeys = newAnimNode->m_numRotationKeys;
        for (uint32 key = 0; key < tempNumRotKeys; ++key) {
          newAnimNode->m_vRotationKeys[key].m_time =
                       g_pAScene->mAnimations[i]->mChannels[j]->mRotationKeys[key].mTime;

          newAnimNode->m_vRotationKeys[key].m_value.x =
                       g_pAScene->mAnimations[i]->mChannels[j]->mRotationKeys[key].mValue.x;

          newAnimNode->m_vRotationKeys[key].m_value.y =
                       g_pAScene->mAnimations[i]->mChannels[j]->mRotationKeys[key].mValue.y;

          newAnimNode->m_vRotationKeys[key].m_value.z =
                       g_pAScene->mAnimations[i]->mChannels[j]->mRotationKeys[key].mValue.z;

          newAnimNode->m_vRotationKeys[key].m_value.w =
                       g_pAScene->mAnimations[i]->mChannels[j]->mRotationKeys[key].mValue.w;
        }

        newAnimNode->m_numScalingKeys = g_pAScene->mAnimations[i]->mChannels[j]->mNumScalingKeys;
        newAnimNode->m_scalingKeys.resize(newAnimNode->m_numScalingKeys);

        uint32 tempNumScalKeys = newAnimNode->m_numScalingKeys;
        for (uint32 key = 0; key < tempNumScalKeys; ++key) {
          newAnimNode->m_scalingKeys[key].m_time =
                       g_pAScene->mAnimations[i]->mChannels[j]->mScalingKeys[key].mTime;

          newAnimNode->m_scalingKeys[key].m_value.x =
                       g_pAScene->mAnimations[i]->mChannels[j]->mScalingKeys[key].mValue.x;

          newAnimNode->m_scalingKeys[key].m_value.y =
                       g_pAScene->mAnimations[i]->mChannels[j]->mScalingKeys[key].mValue.y;

          newAnimNode->m_scalingKeys[key].m_value.z =
                       g_pAScene->mAnimations[i]->mChannels[j]->mScalingKeys[key].mValue.z;
        }

        newAnimation->m_vChannels[j] = newAnimNode;
      }

      SPtr<AnimationData> anim(newAnimation);
      m_vAnimationData.push_back(anim);
    }

    if (0 < g_pAScene->mNumAnimations) {
      m_pAnimation->setNumAnims((uint32)g_pAScene->mNumAnimations);
    }
  }

  Vector<Textures*> 
  ResourceManager::loadMaterialTextures(aiMaterial* pAMat, aiTextureType Atype) {
    Vector<Textures*> textures;

    uint32 tempTypeTexture = pAMat->GetTextureCount(Atype);
    for (uint32 i = 0; i < tempTypeTexture; ++i) {
      aiString aistr;

      pAMat->GetTexture(Atype, i, &aistr);

      String srcFile = String(aistr.C_Str());
      srcFile = m_texturesDirectory + getTexturePath(srcFile);

      srcFile = deleteSlashs(srcFile);

      bool skip = false;

      uint32 tempTextureSize = m_vTextures.size();
      for (uint32 j = 0; j < tempTextureSize; ++j) {
        if (nullptr != m_vTextures[j]) {
          textures.push_back(m_vTextures[j]);
          skip = true;
          break;
        }
      }
      if (!skip) {
        auto myGraphicApi = g_graphicApi().instancePtr();

        m_vTextures.push_back(myGraphicApi->loadTextureFromFile(srcFile));
        textures.push_back(myGraphicApi->loadTextureFromFile(srcFile));
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
  ResourceManager::deleteSlashs(String file) {
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
  ResourceManager::loadTexture(String path, TYPE_TEXTURES::E typeTexture) {
    auto myGraphicApi = g_graphicApi().instancePtr();

    switch (typeTexture)
    {
      case gaEngineSDK::TYPE_TEXTURES::kDiffuse:
        break;
      case gaEngineSDK::TYPE_TEXTURES::kAlbedo:
        break;
      case gaEngineSDK::TYPE_TEXTURES::kMetallic:
        break;
      case gaEngineSDK::TYPE_TEXTURES::kNormal:
        break;
      case gaEngineSDK::TYPE_TEXTURES::kRoughness:
        break;
      case gaEngineSDK::TYPE_TEXTURES::kSpecular:
        break;
      case gaEngineSDK::TYPE_TEXTURES::kAO:
        break;
      case gaEngineSDK::TYPE_TEXTURES::kEmissive:
        break;
      default:
        break;
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

  Mesh* 
  ResourceManager::getMesh() {
    return m_newMesh;
  }

  String
  ResourceManager::getModelDirectory() {
    return m_modelDirectory;
  }

  Vector<Textures*>
  ResourceManager::getVecTextures() {
    return m_vTextures;
  }

  void
  ResourceManager::createDirectories(String const& path) {
    //Retrieve the directory path of the _file
    m_modelDirectory = path.substr(0, path.find_last_of('/'));

    //This is to get the path of the textures.
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
    m_texturesDirectory += miniPaht + '/';
    miniPaht.clear();
  }
}