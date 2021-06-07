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

    for (unsigned int i = 0; i < m_textures.size(); i++) {
      myGraphicApi->setSamplerState(0, pSamplerState, m_textures[i].texture);
      myGraphicApi->setShaderResourceView(m_textures[i].texture, i, 1);
    }

    myGraphicApi->setVertexBuffer(*m_pVertexBuffer);
    myGraphicApi->setIndexBuffer(*m_pIndexBuffer);
    myGraphicApi->drawIndex(m_vIndices.size(), 0, 0);
  }
}