#pragma once

#include "gaPrerequisitesCore.h"
#include "gaStructures.h"
#include "gaSamplerState.h"

namespace gaEngineSDK {
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
    uint32 numBones;
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
      init(Vector<Vertex::E> pVertices, Vector<uint32> pIndices,
           Vector<Texture::E> textures);

      /*
      * @brief Function to save information for m_vertexBuffer and m_indexBuffer.
      * @param Variable with API information.
      */
      void 
      setUpMesh();
      
      /*
      * @brief Function to save vertices and indices and finally draw.
      * @param Variable with API information.
      */
      void 
      draw(Vector <SamplerState*> pSamplerState);

      /***********************************************************************/
      /*
      * Sets.
      */
      /***********************************************************************/

      /*
      * @brief .
      */
      void
      setVertexData(WeakSPtr<Vertex::E> vertexData);

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

      /*
      * @brief Member to store vertex information.
      */
      VertexBuffer* m_pVertexBuffer;

      /*
      * @brief Member to store the index information.
      */
      IndexBuffer* m_pIndexBuffer;

      /*
      * @brief .
      */
      SPtr<SkeletalMesh> m_skeletalMesh = nullptr;

      /*
      * @brief .
      */
      Vector<Matrix4x4> m_bonesTransforms;

    private:
      /***********************************************************************/
      /*
      * Members.
      */
      /***********************************************************************/

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
      uint32 m_numIndices;

      /*
      * @brief .
      */
      uint32 m_numVertices;

      /*
      * @brief .
      */
      String m_meshName;

      /*
      * @brief Member to store the texture information.
      */
      Vector<Texture::E> m_textures;

      /*
      * @brief .
      */
      Vector<Matrix4x4> m_bonesTransform;

      /*
      * @brief Member to store vertex information.
      */
      Vector<Vertex::E> m_vVertices;

      /*
      * @brief Member to store index information.
      */
      Vector<uint32> m_vIndices;

      /*
      * @brief .
      */
      Model* m_pModel;

      /*
      * @brief Member to save the info of this type of texture.
      */
      SPtr<Textures> m_diffuse;

      /*
      * @brief Member to save the info of this type of texture.
      */
      SPtr<Textures> m_albedo;

      /*
      * @brief Member to save the info of this type of texture.
      */
      SPtr<Textures> m_normals;

      /*
      * @brief Member to save the info of this type of texture.
      */
      SPtr<Textures> m_specular;

      /*
      * @brief Member to save the info of this type of texture.
      */
      SPtr<Textures> m_metalness;

      /*
      * @brief Member to save the info of this type of texture.
      */
      SPtr<Textures> m_roughness;

      /*
      * @brief .
      */
      SPtr<Vertex::E> m_vertexData = nullptr;

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