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
}