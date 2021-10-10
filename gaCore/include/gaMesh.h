#pragma once

#include <gaQuaternions.h>

#include "gaPrerequisitesCore.h"
#include "gaStructures.h"
#include "gaSamplerState.h"

namespace gaEngineSDK {
  class VertexBuffer;
  class GraphicsApi;
  class IndexBuffer;
  class Textures;
  class Models;

  struct ModelNodes;
  struct AnimationNode;
  struct AnimationData;

  struct Bone {
    Matrix4x4 offSet;
    Matrix4x4 transformation;
  };

  struct SkeletalMesh {
    uint32 numBones = 0;
    Map<String, int32> bonesMap;
    Vector<Bone> vBones;
  };

  class GA_CORE_EXPORT Mesh
  {
   public:
    /*************************************************************************/
    /*
    * Constructor and destructor.
    */
    /*************************************************************************/
    Mesh() = default;

    ~Mesh() = default;

    /*************************************************************************/
    /*
    * Methods.
    */
    /*************************************************************************/

    /*
    * @brief Function to save information for m_vertexBuffer and m_indexBuffer.
    * @param Variable with API information.
    */
    void
    setUpMesh(Vector<Vertex> pVertices, Vector<uint32> pIndices, Vector<Textures*> textures);

    void
    animated(const float& animationTime,
             SPtr<AnimationData> animation,
             WeakSPtr<ModelNodes> structModelNode);

    void
    boneTransform(const float& deltaTime,
                  SPtr<AnimationData> animation,
                  WeakSPtr<ModelNodes> structModelNode);

    void
    readNodeHierarchy(const float& animationTime,
                      WeakSPtr<ModelNodes> structModelNode,
                      const Matrix4x4& parentTransform,
                      SPtr<AnimationData> animation);

    const AnimationNode*
    findNodeAnim(AnimationData* animation, const String& nodeName);

    Vector3
    calcInterpolatedScaling(const float& animationTime, const AnimationNode* animationNode);

    Matrix4x4
    calcInterpolatedRotation(const float& animationTime, const AnimationNode* animationNode);

    Vector3
    calcInterpolatedPosition(const float& animationTime, const AnimationNode* animationNode);

    uint32
    findScaling(const float& animationTime, const AnimationNode* animationNode);

    uint32
    findRotation(const float& animationTime, const AnimationNode* animationNode);

    uint32
    findPosition(const float& animationTime, const AnimationNode* animationNode);

    Matrix4x4
    nlerp(Quaternions a, Quaternions b, const float& blend);

    /*************************************************************************/
    /*
    * Sets.
    */
    /*************************************************************************/

    /*
    * @brief .
    */
    void
    setVertexData(WeakSPtr<Vertex> vertexData);

    /*
    * @brief .
    */
    void
    setVertexBuffer(WeakSPtr<VertexBuffer> vertexBuffer);

    /*
    * @brief .
    */
    void
    setIndexBuffer(WeakSPtr<IndexBuffer> indexBuffer);

    /*
    * @brief .
    */
    void
    setIndex(WeakSPtr<uint32> index);

    /*
    * @brief .
    */
    void
    setIndexNum(uint32 indexNum);

    /*
    * @brief .
    */
    void
    setVerticesNum(uint32 verticesNum);

    void
    setMeshName(String name);

    /*************************************************************************/
    /*
    * Gets.
    */
    /*************************************************************************/

    Vector<Vertex>
    getVecVertex();

    Vector<uint32>
    getVecIndex();

    uint32
    getVertices();

    uint32
    getNumIndices();

    SPtr<VertexBuffer>
    getVertexBuffer();

    SPtr<IndexBuffer>
    getIndexBuffer();

    /*************************************************************************/
    /*
    * Members.
    */
    /*************************************************************************/

    /*
    * @brief .
    */
    Vector<Matrix4x4> m_bonesTransforms;

    /*
    * @brief .
    */
    cbModelData* m_cbModelData;

    /*
    * @brief Member to store the texture information.
    */
    Vector<Textures*> m_vTextures;

    /*
    * @brief Member to store vertex information.
    */
    SPtr<VertexBuffer> m_pVertexBuffer;

    /*
    * @brief Member to store the index information.
    */
    SPtr<IndexBuffer> m_pIndexBuffer;

    /*
    * @brief .
    */
    SPtr<SkeletalMesh> m_skeletalMesh;

   private:
    /*
    * @brief .
    */
    uint32 m_numIndices = 0;

    /*
    * @brief .
    */
    uint32 m_numVertices = 0;

    /*
    * @brief .
    */
    String m_meshName = "Default_Name_Mesh";

    /*
    * @brief .
    */
    Vector<Matrix4x4> m_bonesTransform;

    /*
    * @brief Member to store vertex information.
    */
    Vector<Vertex> m_vVertices;

    /*
    * @brief Member to store index information.
    */
    Vector<uint32> m_vIndices;

    /*
    * @brief .
    */
    SPtr<Vertex> m_vertexData;

    /*
    * @brief .
    */
    SPtr<uint32> m_pIndex;
  };
}