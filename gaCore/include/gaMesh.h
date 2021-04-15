#pragma once

#include "gaPrerequisitesCore.h"
#include "gaStructures.h"

namespace gaEngineSDK {

  class GraphicsApi;
  class VertexBuffer;
  class IndexBuffer;
  class SamplerState;

  class Mesh
  {
    public:
      /***********************************************************************/
      /*
      * Constructor and destructor.
      */
      /***********************************************************************/
      Mesh() = default;
    
      virtual
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
      init(Vector<Vertex::E>* pVertices, Vector<uint32>* pIndices,
           Vector<Texture::E> textures, GraphicsApi* pGraphicApi);

      /*
      * @brief Function to save information for m_vertexBuffer and m_indexBuffer.
      * @param Variable with API information.
      */
      void 
      setUpMesh(GraphicsApi* pGraphicApi);
      
      /*
      * @brief Function to save vertices and indices and finally draw.
      * @param Variable with API information.
      */
      void 
      draw(GraphicsApi* pGraphicApi, Vector <SamplerState*> pSamplerState);

    private:
      /***********************************************************************/
      /*
      * Members.
      */
      /***********************************************************************/
      
      /*
      * @brief Member to store vertex information.
      */
      VertexBuffer* m_pVertexBuffer;
      
      /*
      * @brief Member to store the index information.
      */
      IndexBuffer* m_pIndexBuffer;
      
      /*
      * @brief Member to store vertex information.
      */
      Vector<Vertex::E>* m_pVertices;
      
      /*
      * @brief Member to store index information.
      */
      Vector<uint32>* m_pIndices;
      
      /*
      * @brief Member to store the texture information.
      */
      Vector<Texture::E> m_textures;
  };
}