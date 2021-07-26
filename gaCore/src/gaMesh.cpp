#include "gaMesh.h"
#include "gaGraphicsApi.h"
#include "gaModels.h"
#include "gaResourceManager.h"

namespace gaEngineSDK {
  void 
  Mesh::setUpMesh(Vector<Vertex> pVertices, Vector<uint32> pIndices, 
                  Vector<Textures*> textures) {
    m_vVertices = pVertices;
    m_vIndices = pIndices;
    m_vTextures = textures;

    auto myGraphicApi = g_graphicApi().instancePtr();

    m_pVertexBuffer.reset(myGraphicApi->createVertexBuffer(m_vVertices.data(),
                          sizeof(Vertex) * m_vVertices.size()));

    m_pIndexBuffer.reset(myGraphicApi->createIndexBuffer(m_vIndices.data(),
                         sizeof(uint32) * m_vIndices.size()));
  }

  void
  Mesh::animated(const float& animationTime, SPtr<AnimationData> animation, 
                 WeakSPtr<ModelNodes> structModelNode) {
      boneTransform(animationTime, animation, structModelNode);
  }

  void 
  Mesh::boneTransform(const float& deltaTime, SPtr<AnimationData> animation,
                      WeakSPtr<ModelNodes> structModelNode) {
    Matrix4x4 identityMatrix;

    float timInTicks = deltaTime * animation->m_ticksPerSecond;
    float timeAnimation = fmod(timInTicks, (float)animation->m_animDuration);

    readNodeHierarchy(timeAnimation, structModelNode, identityMatrix, animation);
  }

  void 
  Mesh::readNodeHierarchy(const float& animationTime, WeakSPtr<ModelNodes> structModelNode,
                          const Matrix4x4& parentTransform, SPtr<AnimationData> animation) {
    ModelNodes* animNode = structModelNode.lock().get();
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
      Matrix4x4 globalInverseTransform;

      m_cbBonesTrans->bonesTransform[bonesIndex] = globalInverseTransform * globalTransform * 
                                                   m_skeletalMesh->vBones[bonesIndex].offSet;
    }

    uint32 tempNumChildrens = animNode->m_numChildrens;
    for (uint32 i = 0; i < tempNumChildrens; ++i) {
      readNodeHierarchy(animationTime, animNode->m_vChildrenNodes[i], globalTransform, 
                        animation);
    }
  }

  const AnimationNode* 
  Mesh::findNodeAnim(AnimationData* animation, const String& nodeName) {
    //Channel in animation contains aiNodeAnim (aiNodeAnim its transformation for bones)
    uint32 tempNumChannels = animation->m_numChannels;
    for (uint32 i = 0; i < tempNumChannels; ++i) {
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
      scaling.x = animationNode->m_scalingKeys[0].m_value.x;
      scaling.y = animationNode->m_scalingKeys[0].m_value.y;
      scaling.z = animationNode->m_scalingKeys[0].m_value.z;
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
    start.x = animationNode->m_scalingKeys[scalingIndex].m_value.x;
    start.y = animationNode->m_scalingKeys[scalingIndex].m_value.y;
    start.z = animationNode->m_scalingKeys[scalingIndex].m_value.z;

    Vector3 end;
    end.x = animationNode->m_scalingKeys[nextScalingIndex].m_value.x;
    end.y = animationNode->m_scalingKeys[nextScalingIndex].m_value.y;
    end.z = animationNode->m_scalingKeys[nextScalingIndex].m_value.z;

    Vector3 delta = end - start;

    Vector3 temp;
    temp.x = factor * delta.x;
    temp.y = factor * delta.y;
    temp.z = factor * delta.z;

    scaling = start + temp;

    return scaling;
  }

  Matrix4x4 
  Mesh::calcInterpolatedRotation(const float& animationTime, const AnimationNode* animationNode) {
    Matrix4x4 final;

    if (1 == animationNode->m_numRotationKeys) {
      final = animationNode->m_vRotationKeys[0].m_value.getMatrix();
      final.transpose();
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
      pos.x = animationNode->m_vPositionKeys[0].m_value.x;
      pos.y = animationNode->m_vPositionKeys[0].m_value.y;
      pos.z = animationNode->m_vPositionKeys[0].m_value.z;
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
    start.x = animationNode->m_vPositionKeys[positionIndex].m_value.x;
    start.y = animationNode->m_vPositionKeys[positionIndex].m_value.y;
    start.z = animationNode->m_vPositionKeys[positionIndex].m_value.z;

    Vector3 end;
    end.x = animationNode->m_vPositionKeys[nextPositionIndex].m_value.x;
    end.y = animationNode->m_vPositionKeys[nextPositionIndex].m_value.y;
    end.z = animationNode->m_vPositionKeys[nextPositionIndex].m_value.z;

    Vector3 delta = end - start;
    
    Vector3 temp;
    temp.x = factor * delta.x;
    temp.y = factor * delta.y;
    temp.z = factor * delta.z;

    pos = start + temp;

    return pos;
  }

  uint32 
  Mesh::findScaling(const float& animationTime, const AnimationNode* animationNode) {
    uint32 tempNumScalingKeys = animationNode->m_numScalingKeys - 1;
    for (uint32 i = 0; i < tempNumScalingKeys; ++i) {
      if (animationTime < (float)animationNode->m_scalingKeys[i + 1].m_time) {
        return i;
      }
    }

    GA_ASSERT(0);
    return 0;
  }

  uint32 
  Mesh::findRotation(const float& animationTime, const AnimationNode* animationNode) {
    uint32 tempNumRotKeys = animationNode->m_numRotationKeys - 1;
    for (uint32 i = 0; i < tempNumRotKeys; ++i) {
      if (animationTime < (float)animationNode->m_vRotationKeys[i + 1].m_time) {
        return i;
      }
    }

    GA_ASSERT(0);
    return 0;
  }

  uint32 
  Mesh::findPosition(const float& animationTime, const AnimationNode* animationNode) {
    uint32 tempNumPosKeys = animationNode->m_numPositionKeys - 1;
    for (uint32 i = 0; i < tempNumPosKeys; ++i) {
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

    float dotProduct = a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
    float oneMinusBlend = 1.0f - blend;

    if (dotProduct < 0.0f) {
      result.x = a.x * oneMinusBlend + blend * -b.x;
      result.y = a.y * oneMinusBlend + blend * -b.y;
      result.z = a.z * oneMinusBlend + blend * -b.z;
      result.w = a.w * oneMinusBlend + blend * -b.w;
    }
    else {
      result.x = a.x * oneMinusBlend + blend * b.x;
      result.y = a.y * oneMinusBlend + blend * b.y;
      result.z = a.z * oneMinusBlend + blend * b.z;
      result.w = a.w * oneMinusBlend + blend * b.w;
    }
    result.normalize();

    Matrix4x4 final;
    final = result.getMatrix();
    final.transpose();

    return final;
  }

  /***************************************************************************/
  /*
  * Sets.
  */
  /***************************************************************************/

  void 
  Mesh::setVertexData(WeakSPtr<Vertex> vertexData) {
    m_vertexData = vertexData.lock();
  }

  void
  Mesh::setVertexBuffer(WeakSPtr<VertexBuffer> vertexBuffer) {
    m_vertexBuffer = vertexBuffer.lock();
  }

  void 
  Mesh::setIndexBuffer(WeakSPtr<IndexBuffer> indexBuffer) {
    m_indexBuffer = indexBuffer.lock();
  }

  void 
  Mesh::setIndexNum(uint32 indexNum) {
    m_numIndices = indexNum;
  }

  void 
  Mesh::setVerticesNum(uint32 verticesNum) {
    m_numVertices = verticesNum;
  }

  /***************************************************************************/
  /*
  * Gets.
  */
  /***************************************************************************/

  Vector<Vertex> 
  Mesh::getVecVertex() {
    return m_vVertices;
  }

  Vector<uint32> 
  Mesh::getVecIndex() {
    return m_vIndices;
  }

  uint32
  Mesh::getVertices() {
    return m_numVertices;
  }

  uint32 
  Mesh::getNumIndices() {
    m_numIndices = m_vIndices.size();
    return m_numIndices;
  }
}