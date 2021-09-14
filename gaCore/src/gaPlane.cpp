#include "gaPlane.h"
#include "gaModels.h"
#include "gaGraphicsApi.h"

namespace gaEngineSDK {
  void 
  Plane::createSAQ() {
    auto myGraphicsApi = g_graphicApi().instancePtr();
    Models* newModel = new Models();
    Mesh* newMesh = new Mesh();

    Vertex* meshVertex = new Vertex[4];
    meshVertex[0].position = { -1.0f, -1.0f, 0.0f, 1.0f };
    meshVertex[1].position = { -1.0f,  1.0f, 0.0f, 1.0f };
    meshVertex[2].position = { 1.0f, -1.0f, 0.0f, 1.0f };
    meshVertex[3].position = { 1.0f,  1.0f, 0.0f, 1.0f };

    meshVertex[0].normal = { 0.0f, 0.0f, -1.0f };
    meshVertex[1].normal = { 0.0f, 0.0f, -1.0f };
    meshVertex[2].normal = { 0.0f, 0.0f, -1.0f };
    meshVertex[3].normal = { 0.0f, 0.0f, -1.0f };

    meshVertex[0].texCoords = { 0.0f, 1.0f };
    meshVertex[1].texCoords = { 0.0f, 0.0f };
    meshVertex[2].texCoords = { 1.0f, 1.0f };
    meshVertex[3].texCoords = { 1.0f, 0.0f };

    uint32* meshIndex = new uint32[2 * 3];
    meshIndex[0] = 0;
    meshIndex[1] = 1;
    meshIndex[2] = 2;
    meshIndex[3] = 2;
    meshIndex[4] = 1;
    meshIndex[5] = 3;

    SPtr<Vertex> spVetexData(meshVertex);
    newMesh->setVertexData(spVetexData);

    newMesh->m_pVertexBuffer.reset(myGraphicsApi->createVertexBuffer(meshVertex, 
                                                                     sizeof(Vertex) * 4));
    SPtr<uint32> spIndex(meshIndex);
    newMesh->setIndex(spIndex);
    newMesh->setIndexNum(6);

    SPtr<IndexBuffer> tempIB(myGraphicsApi->createIndexBuffer(meshIndex, sizeof(uint32) * 6));
    newMesh->setIndexBuffer(tempIB);

    newModel->addNewMesh(*newMesh);
    m_SAQ.reset(newModel);
  }

  void
  Plane::setSAQ() {
    auto myGraphicsApi = g_graphicApi().instancePtr();
    auto mesh = m_SAQ->getMesh(0);

    myGraphicsApi->setVertexBuffer(mesh.getVertexBuffer());
    myGraphicsApi->setIndexBuffer(mesh.getIndexBuffer());
    myGraphicsApi->drawIndex(6);
  }
}