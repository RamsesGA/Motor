#pragma once

#include <gaQuaternions.h>

#include "gaPrerequisitesCore.h"
#include "gaStructures.h"
#include "gaSamplerState.h"

namespace gaEngineSDK {
  class ResourceManager;
  class VertexBuffer;
  class GraphicsApi;
  class IndexBuffer;
  class Textures;
  class Model;

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

  class Mesh
  {
    public:
      /***********************************************************************/
      /*
      * Constructor and destructor.
      */
      /***********************************************************************/
      Mesh() = default;
    
      ~Mesh() = default;

      /***********************************************************************/
      /*
      * Methods.
      */
      /***********************************************************************/
      
      /*
      * @brief Function to initialize class members.
      * @param Variable with API information.
      */
      void 
      init(Vector<Vertex> pVertices, Vector<uint32> pIndices,
           Vector<Texture> textures);

      /*
      * @brief Function to save information for m_vertexBuffer and m_indexBuffer.
      * @param Variable with API information.
      */
      void 
      setUpMesh();

      void 
      animated(ResourceManager& resource, const float& animationTime, SPtr<AnimationData> animation);

      void 
      boneTransform(ResourceManager& resource, const float& deltaTime, SPtr<AnimationData> animation);

      void
      readNodeHierarchy(ResourceManager& resource, 
                        const float& animationTime, WeakSPtr<ModelNodes> node,
                        const Matrix4x4& parentTransform, SPtr<AnimationData> animation);

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

      /***********************************************************************/
      /*
      * Sets.
      */
      /***********************************************************************/

      /*
      * @brief .
      */
      void
      setVertexData(WeakSPtr<Vertex> vertexData);

      /*
      * @brief .
      */
      void
      setVertexBuffer(VertexBuffer* vertexBuffer);

      /*
      * @brief .
      */
      void
      setIndex(WeakSPtr<uint32> index);

      /*
      * @brief .
      */
      void
      setIndexBuffer(IndexBuffer* indexBuffer);

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

      /*
      * @brief .
      */
      void
      setDiffuseTexture(WeakSPtr<Textures> diffuse);

      /*
      * @brief .
      */
      void
      setAlbedoTexture(WeakSPtr<Textures> albedo);

      /*
      * @brief .
      */
      void
      setNormalsTexture(WeakSPtr<Textures> normals);

      /*
      * @brief .
      */
      void
      setSpecularTexture(WeakSPtr<Textures> specular);

      /*
      * @brief .
      */
      void
      setMetalnessTexture(WeakSPtr<Textures> metal);

      /*
      * @brief .
      */
      void
      setRoughnessTexture(WeakSPtr<Textures> roughness);


      /***********************************************************************/
      /*
      * Gets.
      */
      /***********************************************************************/

      uint32
      getVertices();

      uint32
      getNumIndices();

      SPtr<Textures> 
      getDiffuse();

      SPtr<Textures> 
      getAlbedo();

      SPtr<Textures> 
      getNormals();

      SPtr<Textures> 
      getSpecular();

      SPtr<Textures> 
      getMetalness();

      SPtr<Textures> 
      getRoughness();

      /***********************************************************************/
      /*
      * Members.
      */
      /***********************************************************************/

      /*
      * @brief Member to store vertex information.
      */
      SPtr<VertexBuffer> m_pVertexBuffer = nullptr;

      /*
      * @brief Member to store the index information.
      */
      SPtr<IndexBuffer> m_pIndexBuffer = nullptr;

      /*
      * @brief .
      */
      SPtr<SkeletalMesh> m_skeletalMesh = nullptr;

      /*
      * @brief .
      */
      Vector<Matrix4x4> m_bonesTransforms;

      /*
      * @brief .
      */
      ConstBuffBonesTransform* m_cbBonesTransform = nullptr;

      /*
      * @brief Member to store the texture information.
      */
      Vector<Texture> m_textures;

    private:

      /*
      * @brief
      */
      bool m_hasDiffuse = false;

      /*
      * @brief
      */
      bool m_hasAlbedo = false;

      /*
      * @brief
      */
      bool m_hasNormal = false;

      /*
      * @brief
      */
      bool m_hasSpecular = false;

      /*
      * @brief
      */
      bool m_hasMetalness = false;

      /*
      * @brief
      */
      bool m_hasRoughness = false;

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
      * @brief Member to save the info of this type of texture.
      */
      SPtr<Textures> m_diffuse = nullptr;

      /*
      * @brief Member to save the info of this type of texture.
      */
      SPtr<Textures> m_albedo = nullptr;

      /*
      * @brief Member to save the info of this type of texture.
      */
      SPtr<Textures> m_normals = nullptr;

      /*
      * @brief Member to save the info of this type of texture.
      */
      SPtr<Textures> m_specular = nullptr;

      /*
      * @brief Member to save the info of this type of texture.
      */
      SPtr<Textures> m_metalness = nullptr;

      /*
      * @brief Member to save the info of this type of texture.
      */
      SPtr<Textures> m_roughness = nullptr;

      /*
      * @brief .
      */
      SPtr<Vertex> m_vertexData = nullptr;

      /*
      * @brief .
      */
      SPtr<VertexBuffer> m_vertexBuffer = nullptr;

      /*
      * @brief .
      */
      SPtr<IndexBuffer> m_indexBuffer = nullptr;

      /*
      * @brief .
      */
      SPtr<uint32> m_index = nullptr;
  };
}