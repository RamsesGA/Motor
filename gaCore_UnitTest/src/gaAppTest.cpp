#include <Windows.h>
#include <gaPlane.h>
#include <gaModels.h>
#include <gaTextures.h>
#include <gaMaterials.h>
#include <gaSceneGraph.h>
#include <gaStaticMesh.h>
#include <gaResourceManager.h>
#include <gaLights.h>
#include <gaBaseOmniConnect.h>
#include <gaBaseInterface.h>

#include "gaAppTest.h"

using std::pair;

void
AppTest::onUpdate(float deltaTime) {
  //auto myGraphicsApi = g_graphicApi().instancePtr();
  auto mySceneGraph = SceneGraph::instancePtr();

  //Update the nodes info.
  mySceneGraph->update(deltaTime);
}

void
AppTest::onRender() {
  auto myGraphicsApi = g_graphicApi().instancePtr();

  // Present our back buffer to our front buffer
  myGraphicsApi->swapChainPresent();
}

void
AppTest::onCreate() {
  /*
  * D 3 D 1 2
  */
  //myGraphicsApi->initDevice(m_sfmlWindow.getSystemHandle());

  /*
  * D 3 D 1 1
  */
  auto myGraphicsApi = g_graphicApi().instancePtr();

  //We send the window to the API.
  myGraphicsApi->initDevice(m_sfmlWindow.getSystemHandle());
}