#include "gaMesh.h"
#include "gaGraphicsApi.h"

namespace gaEngineSDK {
  void 
  Mesh::init(Vector<Vertex::E> pVertices, Vector<uint32> pIndices, 
             Vector<Texture::E> textures, GraphicsApi* pGraphicApi) {
    m_vVertices = pVertices;
    m_vIndices = pIndices;
    m_textures = textures;

    setUpMesh(pGraphicApi);
  }

  void 
  Mesh::setUpMesh(GraphicsApi* pGraphicApi) {
    m_pVertexBuffer = pGraphicApi->createVertexBuffer(m_vVertices.data(),
                                                      sizeof(Vertex::E) * m_vVertices.size());

    m_pIndexBuffer = pGraphicApi->createIndexBuffer(m_vIndices.data(),
                                                    sizeof(uint32) * m_vIndices.size());
  }

  void 
  Mesh::draw(GraphicsApi* pGraphicApi, Vector<SamplerState*> pSamplerState) {
    for (unsigned int i = 0; i < m_textures.size(); i++) {
      //pGraphicApi->setSamplerState(0, pSamplerState, *m_textures[i].texture);
      //
      //pGraphicApi->setShaderResourceView(m_textures[i].texture, i, 0);
    }

    pGraphicApi->setVertexBuffer(*m_pVertexBuffer);
    pGraphicApi->setIndexBuffer(*m_pIndexBuffer);
    pGraphicApi->drawIndex(m_vIndices.size(), 0, 0);
  }
}