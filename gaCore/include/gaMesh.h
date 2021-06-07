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

      /*
      * @brief Member to store vertex information.
      */
      VertexBuffer* m_pVertexBuffer;

      /*
      * @brief Member to store the index information.
      */
      IndexBuffer* m_pIndexBuffer;

    private:
      /***********************************************************************/
      /*
      * Members.
      */
      /***********************************************************************/

      /*
      * @brief .
      */
      uint32 m_numIndices;

      /*
      * @brief .
      */
      uint32 m_numVertices;
      
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
      SPtr<Textures>m_normals;

      /*
      * @brief Member to save the info of this type of texture.
      */
      SPtr<Textures> m_specular;

      /*
      * @brief Member to save the info of this type of texture.
      */
      SPtr<Textures>m_metalness;

      /*
      * @brief Member to save the info of this type of texture.
      */
      SPtr<Textures> m_roughness;

      /*
      * @brief .
      */
      SPtr<SkeletalMesh> m_skeletalMesh;
  };
}