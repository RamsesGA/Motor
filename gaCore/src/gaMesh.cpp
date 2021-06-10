#include "gaMesh.h"
#include "gaGraphicsApi.h"
#include "gaModels.h"

namespace gaEngineSDK {
  void 
  Mesh::init(Vector<Vertex::E> pVertices, Vector<uint32> pIndices, 
             Vector<Texture::E> textures) {
    m_vVertices = pVertices;
    m_vIndices = pIndices;
    m_textures = textures;

    setUpMesh();
  }

  void 
  Mesh::setUpMesh() {
    auto myGraphicApi = g_graphicApi().instancePtr();

    m_pVertexBuffer = myGraphicApi->createVertexBuffer(m_vVertices.data(),
                                                       sizeof(Vertex::E) * m_vVertices.size());

    m_pIndexBuffer = myGraphicApi->createIndexBuffer(m_vIndices.data(),
                                                     sizeof(uint32) * m_vIndices.size());
  }

  void 
  Mesh::draw(Vector <SamplerState*> pSamplerState) {
    auto myGraphicApi = g_graphicApi().instancePtr();

    for (uint32 i = 0; i < m_textures.size(); i++) {
      myGraphicApi->setSamplerState(0, pSamplerState, m_textures[i].texture);
      myGraphicApi->setShaderResourceView(m_textures[i].texture, i, 1);
    }

    myGraphicApi->setVertexBuffer(*m_pVertexBuffer);
    myGraphicApi->setIndexBuffer(*m_pIndexBuffer);
    myGraphicApi->drawIndex(m_vIndices.size(), 0, 0);
  }

  void
  Mesh::animated(const float& animationTime, SPtr<AnimationData> animation) {
    if (nullptr != animation) {
      boneTransform(animationTime, animation);
    }
  }

  void 
  Mesh::boneTransform(const float& deltaTime, SPtr<AnimationData> animation) {
    Matrix4x4 identityMatrix;
    identityMatrix.identity();

    float timInTicks = deltaTime * animation->m_ticksPerSecond;
    float timeAnimation = fmod(timInTicks, (float)animation->m_animDuration);

    readNodeHierarchy(timeAnimation, m_pModel->m_modelNodes, identityMatrix, animation);
  }

  void 
  Mesh::readNodeHierarchy(const float& animationTime, WeakSPtr<ModelNodes> node, 
                          const Matrix4x4& parentTransform, SPtr<AnimationData> animation) {
    ModelNodes* animNode = node.lock().get();
    String nodeName(animNode->m_name);

    Matrix4x4 nodeTransform;

    std::memcpy(&nodeTransform, &animNode->m_transformMatrix, sizeof(Matrix4x4));

    const AnimationNode* animationNode = findNodeAnim(animation.get(), nodeName);

    if (animationNode) {
      //scaling
      Vector3 scalingVector = calcInterpolatedScaling(animationTime, animationNode);
      Matrix4x4 scalingMatrix;

      scalingMatrix.scale(scalingVector);

      //rotation
      Matrix4x4 rotationMatrix = calcInterpolatedRotation(animationTime, animationNode);

      //translation
      Vector3 translateVector = calcInterpolatedPosition(animationTime, animationNode);
      Matrix4x4 translateMatrix;
      translateMatrix.translate(translateVector);
      nodeTransform = translateMatrix * rotationMatrix * scalingMatrix;
    }

    Matrix4x4 globalTransform = parentTransform * nodeTransform;

    //True if nodeName exist in bone_mapping	
    if (m_skeletalMesh->bonesMap.find(nodeName) != m_skeletalMesh->bonesMap.end()) {
      uint32 bonesIndex = m_skeletalMesh->bonesMap[nodeName];

      m_cbBonesTransform->bonesTransform[bonesIndex] = m_pModel->m_globalInverseTransform *
                                                       globalTransform * 
                                                       m_skeletalMesh->vBones[bonesIndex].offSet;
    }

    for (uint32 i = 0; i < animNode->m_numChildrens; ++i) {
      readNodeHierarchy(animationTime, animNode->m_vChildrenNodes[i], globalTransform, animation);
    }
  }

  const AnimationNode* 
  Mesh::findNodeAnim(AnimationData* animation, const String& nodeName) {
    //Channel in animation contains aiNodeAnim (aiNodeAnim its transformation for bones)
    for (uint32 i = 0; i < animation->m_numChannels; ++i) {
      const AnimationNode* animNode = animation->m_vChannels[i].get();

      if (nodeName == animNode->m_nodeName) {
        return animNode;
      }
    }
    return nullptr;
  }

  Vector3
  Mesh::calcInterpolatedScaling(const float& animationTime, const AnimationNode* animationNode) {
    Vector3 scaling;

    if (1 == animationNode->m_numScalingKeys) {
      scaling.m_x = animationNode->m_scalingKeys[0].m_value.m_x;
      scaling.m_y = animationNode->m_scalingKeys[0].m_value.m_y;
      scaling.m_z = animationNode->m_scalingKeys[0].m_value.m_z;
      return scaling;
    }

    uint32 scalingIndex = findScaling(animationTime, animationNode);
    uint32 nextScalingIndex = scalingIndex + 1;

    GA_ASSERT(nextScalingIndex < animationNode->m_numScalingKeys);

    float deltaTime = (float)(animationNode->m_scalingKeys[nextScalingIndex].m_time -
                              animationNode->m_scalingKeys[scalingIndex].m_time);

    float factor = (animationTime - (float)animationNode->m_scalingKeys[scalingIndex].m_time) / deltaTime;

    GA_ASSERT(factor >= 0.0f && factor <= 1.0f);

    Vector3 start;
    start.m_x = animationNode->m_scalingKeys[scalingIndex].m_value.m_x;
    start.m_y = animationNode->m_scalingKeys[scalingIndex].m_value.m_y;
    start.m_z = animationNode->m_scalingKeys[scalingIndex].m_value.m_z;

    Vector3 end;
    end.m_x = animationNode->m_scalingKeys[nextScalingIndex].m_value.m_x;
    end.m_y = animationNode->m_scalingKeys[nextScalingIndex].m_value.m_y;
    end.m_z = animationNode->m_scalingKeys[nextScalingIndex].m_value.m_z;

    Vector3 delta = end - start;

    Vector3 temp;
    temp.m_x = factor * delta.m_x;
    temp.m_y = factor * delta.m_y;
    temp.m_z = factor * delta.m_z;

    scaling = start + temp;

    return scaling;
  }

  Matrix4x4 
  Mesh::calcInterpolatedRotation(const float& animationTime, const AnimationNode* animationNode) {
    Matrix4x4 final;

    if (1 == animationNode->m_numRotationKeys) {
      final = animationNode->m_vRotationKeys[0].m_value.getMatrix();
      return final;
    }

    uint32 rotationIndex = findRotation(animationTime, animationNode);
    uint32 nextRotationIndex = rotationIndex + 1;

    GA_ASSERT(nextRotationIndex < animationNode->m_numRotationKeys);

    float deltaTime = (float)(animationNode->m_vRotationKeys[nextRotationIndex].m_time -
                               animationNode->m_vRotationKeys[rotationIndex].m_time);

    float factor = (animationTime - 
                    (float)animationNode->m_vRotationKeys[rotationIndex].m_time) / deltaTime;

    GA_ASSERT(factor >= 0.0f && factor <= 1.0f);

    Quaternions start_quat = animationNode->m_vRotationKeys[rotationIndex].m_value;
    Quaternions end_quat = animationNode->m_vRotationKeys[nextRotationIndex].m_value;

    return nlerp(start_quat, end_quat, factor);
  }

  Vector3 
  Mesh::calcInterpolatedPosition(const float& animationTime, const AnimationNode* animationNode) {
    Vector3 pos;

    if (1 == animationNode->m_numPositionKeys) {
      pos.m_x = animationNode->m_vPositionKeys[0].m_value.m_x;
      pos.m_y = animationNode->m_vPositionKeys[0].m_value.m_y;
      pos.m_z = animationNode->m_vPositionKeys[0].m_value.m_z;
      return pos;
    }

    uint32 positionIndex = findPosition(animationTime, animationNode);
    uint32 nextPositionIndex = positionIndex + 1;

    GA_ASSERT(nextPositionIndex < animationNode->m_numPositionKeys);

    float deltaTime = (float)(animationNode->m_vPositionKeys[nextPositionIndex].m_time -
                              animationNode->m_vPositionKeys[positionIndex].m_time);
    float factor = (animationTime - 
                   (float)animationNode->m_vPositionKeys[positionIndex].m_time) / deltaTime;

    GA_ASSERT(factor >= 0.0f && factor <= 1.0f);

    Vector3 start;
    start.m_x = animationNode->m_vPositionKeys[positionIndex].m_value.m_x;
    start.m_y = animationNode->m_vPositionKeys[positionIndex].m_value.m_y;
    start.m_z = animationNode->m_vPositionKeys[positionIndex].m_value.m_z;

    Vector3 end;
    end.m_x = animationNode->m_vPositionKeys[nextPositionIndex].m_value.m_x;
    end.m_y = animationNode->m_vPositionKeys[nextPositionIndex].m_value.m_y;
    end.m_z = animationNode->m_vPositionKeys[nextPositionIndex].m_value.m_z;

    Vector3 delta = end - start;
    
    Vector3 temp;
    temp.m_x = factor * delta.m_x;
    temp.m_y = factor * delta.m_y;
    temp.m_z = factor * delta.m_z;

    pos = start + temp;

    return pos;
  }

  uint32 
  Mesh::findScaling(const float& animationTime, const AnimationNode* animationNode) {
    for (uint32 i = 0; i < animationNode->m_numScalingKeys - 1; ++i) {
      if (animationTime < (float)animationNode->m_scalingKeys[i + 1].m_time) {
        return i;
      }
    }

    GA_ASSERT(0);
    return 0;
  }

  uint32 
  Mesh::findRotation(const float& animationTime, const AnimationNode* animationNode) {
    for (uint32 i = 0; i < animationNode->m_numRotationKeys - 1; ++i) {
      if (animationTime < (float)animationNode->m_vRotationKeys[i + 1].m_time) {
        return i;
      }
    }

    GA_ASSERT(0);
    return 0;
  }

  uint32 
  Mesh::findPosition(const float& animationTime, const AnimationNode* animationNode) {
    for (uint32 i = 0; i < animationNode->m_numPositionKeys - 1; ++i) {
      if (animationTime < (float)animationNode->m_vPositionKeys[i + 1].m_time) {
        return i;
      }
    }

    GA_ASSERT(0);

    return 0;
  }

  Matrix4x4 
  Mesh::nlerp(Quaternions a, Quaternions b, const float& blend) {
    a.normalize();
    b.normalize();

    Quaternions result;

    float dotProduct = a.m_x * b.m_x + a.m_y * b.m_y + a.m_z * b.m_z + a.m_w * b.m_w;
    float oneMinusBlend = 1.0f - blend;

    if (dotProduct < 0.0f) {
      result.m_x = a.m_x * oneMinusBlend + blend * -b.m_x;
      result.m_y = a.m_y * oneMinusBlend + blend * -b.m_y;
      result.m_z = a.m_z * oneMinusBlend + blend * -b.m_z;
      result.m_w = a.m_w * oneMinusBlend + blend * -b.m_w;
    }
    else {
      result.m_x = a.m_x * oneMinusBlend + blend * b.m_x;
      result.m_y = a.m_y * oneMinusBlend + blend * b.m_y;
      result.m_z = a.m_z * oneMinusBlend + blend * b.m_z;
      result.m_w = a.m_w * oneMinusBlend + blend * b.m_w;
    }
    result.normalize();

    Matrix4x4 final;
    final = result.getMatrix();

    return final;
  }

  /***************************************************************************/
  /*
  * Sets.
  */
  /***************************************************************************/

  void 
  Mesh::setVertexData(WeakSPtr<Vertex::E> vertexData) {
    m_vertexData = vertexData.lock();
  }

  void
  Mesh::setVertexBuffer(VertexBuffer* vertexBuffer) {
    m_vertexBuffer.reset(vertexBuffer);
  }

  void 
  Mesh::setIndex(WeakSPtr<uint32> index) {
    m_index = index.lock();
  }

  void 
  Mesh::setIndexBuffer(IndexBuffer* indexBuffer) {
    m_indexBuffer.reset(indexBuffer);
  }

  void 
  Mesh::setIndexNum(uint32 indexNum) {
    m_numIndices = indexNum;
  }

  void 
  Mesh::setVerticesNum(uint32 verticesNum) {
    m_numVertices = verticesNum;
  }

  void
  Mesh::setDiffuseTexture(WeakSPtr<Textures> diffuse) {
    m_hasDiffuse = true;
    m_diffuse = diffuse.lock();
  }

  void 
  Mesh::setAlbedoTexture(WeakSPtr<Textures> albedo) {
    m_hasAlbedo = true;
    m_albedo = albedo.lock();
  }

  void 
  Mesh::setNormalsTexture(WeakSPtr<Textures> normals) {
    m_hasNormal = true;
    m_normals = normals.lock();
  }

  void
  Mesh::setSpecularTexture(WeakSPtr<Textures> specular) {
    m_hasSpecular = true;
    m_specular = specular.lock();
  }

  void 
  Mesh::setMetalnessTexture(WeakSPtr<Textures> metal) {
    m_hasMetalness = true;
    m_metalness = metal.lock();
  }

  void 
  Mesh::setRoughnessTexture(WeakSPtr<Textures> roughness) {
    m_hasRoughness = true;
    m_roughness = roughness.lock();
  }

  /***************************************************************************/
  /*
  * Gets.
  */
  /***************************************************************************/

  uint32
  Mesh::getVertices() {
    return m_numVertices;
  }

  uint32 
  Mesh::getNumIndices() {
    m_numIndices = m_vIndices.size();
    return m_numIndices;
  }

  SPtr<Textures> 
  Mesh::getDiffuse() {
    return m_diffuse;
  }

  SPtr<Textures> 
  Mesh::getAlbedo() {
    return m_albedo;
  }

  SPtr<Textures> 
  Mesh::getNormals() {
    return m_normals;
  }

  SPtr<Textures> 
  Mesh::getSpecular() {
    return m_specular;
  }

  SPtr<Textures> 
  Mesh::getMetalness() {
    return m_metalness;
  }

  SPtr<Textures> 
  Mesh::getRoughness() {
    return m_roughness;
  }
}