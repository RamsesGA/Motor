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
  * D 3 D 1 1
  */
  auto myGraphicsApi = g_graphicApi().instancePtr();

  //We send the window to the API.
  myGraphicsApi->initDevice(m_sfmlWindow.getSystemHandle());

  /***************************************************************************/
  /*
  * Loading models.
  * Set Node
  */
  /***************************************************************************/
  
  createScene();
  //createNodePod();
  //createNodeVela();
  //createNodeTwoB();
  //createNodeUgandan();
  //createNodeGrimoires();
  //createNodeRamlethalSwords();
  //createNodeStranger();
  //createNodesMarco();
  //createNodesMarco2();

  //auto myOmniverse = g_baseOmniConnect().instancePtr();
  //myOmniverse->saveSceneGraphToUSD();

  /*
  * D 3 D 1 2
  */
  //myGraphicsApi->initDevice(m_sfmlWindow.getSystemHandle());
}

/*****************************************************************************/
/*
* Nodes.
*/
/*****************************************************************************/
void
AppTest::createNodePod() {
  auto mySceneGraph = SceneGraph::instancePtr();
  auto myRSRCMG = ResourceManager::instancePtr();

  SPtr<Models> myModel = myRSRCMG->load<Models>("data/models/pod/Pod.fbx");

  SPtr<StaticMesh> myStaticMesh = make_shared<StaticMesh>();
  myStaticMesh->m_pModel = myModel;

  //Creating the component
  SPtr<Component> newComponent(myStaticMesh);

  //Creating actor
  SPtr<Actor> actor(new Actor("Pod"));
  actor->setIsSelected(true);
  actor->setComponent(newComponent);

  //Adding the actor to node root
  mySceneGraph->createNewActor(actor, SPtr<SceneNode>(nullptr));

  //auto myOmniverse = g_baseOmniConnect().instancePtr();
  //myOmniverse->createGeoMeshWithModel(actor);
}

void
AppTest::createNodeVela() {
  auto mySceneGraph = SceneGraph::instancePtr();
  auto myRSRCMG = ResourceManager::instancePtr();

  SPtr<Models> myModel = myRSRCMG->load<Models>("data/models/vela/Vela2.fbx");

  SPtr<StaticMesh> myStaticMesh = make_shared<StaticMesh>();
  myStaticMesh->m_pModel = myModel;

  //Creating the component
  SPtr<Component> newComponent(myStaticMesh);

  //Creating actor
  SPtr<Actor> actor(new Actor("Vela"));
  actor->setIsSelected(true);
  actor->setComponent(newComponent);

  //Adding the actor to node root
  mySceneGraph->createNewActor(actor, SPtr<SceneNode>(nullptr));
}

void
AppTest::createNodeTwoB() {
  auto mySceneGraph = SceneGraph::instancePtr();

  SPtr<Models> myModel = make_shared<Models>();
  myModel->loadFromFile("data/models/2B/2B.obj");

  SPtr<StaticMesh> myStaticMesh = make_shared<StaticMesh>();
  myStaticMesh->m_pModel = myModel;

  //Creating the component
  SPtr<Component> newComponent(myStaticMesh);

  //Creating actor
  SPtr<Actor> actor(new Actor("2B"));
  actor->setIsSelected(true);
  actor->setComponent(newComponent);

  //Adding the actor to node root
  mySceneGraph->createNewActor(actor, SPtr<SceneNode>(nullptr));
}

void
AppTest::createNodeUgandan() {
  auto mySceneGraph = SceneGraph::instancePtr();
  auto myRSRCMG = ResourceManager::instancePtr();

  SPtr<Models> myModel = myRSRCMG->load<Models>("data/models/ugandan/Knuckles.fbx");

  SPtr<StaticMesh> myStaticMesh = make_shared<StaticMesh>();
  myStaticMesh->m_pModel = myModel;

  //Creating the component
  SPtr<Component> newComponent(myStaticMesh);

  //Creating actor
  SPtr<Actor> actor(new Actor("Ugandan"));
  actor->setIsSelected(true);
  actor->setComponent(newComponent);

  //Adding the actor to node root
  mySceneGraph->createNewActor(actor, SPtr<SceneNode>(nullptr));
}

void 
AppTest::createNodeGrimoires() {
  auto mySceneGraph = SceneGraph::instancePtr();
  auto myRSRCMG = ResourceManager::instancePtr();

  SPtr<Models> myModel = myRSRCMG->load<Models>("data/models/grimoires/grimoires.fbx");

  SPtr<StaticMesh> myStaticMesh = make_shared<StaticMesh>();
  myStaticMesh->m_pModel = myModel;

  //Creating the component
  SPtr<Component> newComponent(myStaticMesh);

  //Creating actor
  SPtr<Actor> actor(new Actor("Grimoires"));
  actor->setIsSelected(true);
  actor->setComponent(newComponent);

  //Adding the actor to node root
  mySceneGraph->createNewActor(actor, SPtr<SceneNode>(nullptr));
}

void 
AppTest::createNodeRamlethalSwords() {
  auto mySceneGraph = SceneGraph::instancePtr();
  auto myRSRCMG = ResourceManager::instancePtr();

  SPtr<Models> myModel = myRSRCMG->load<Models>("data/models/ramlethal/Ramlethal Sword.fbx");

  /*
  * M A T E R I A L
  * Z O N E
  */
  Vector<Textures*> vTextures;
  SPtr<Materials> myMaterial = myRSRCMG->load<Materials>
                                         ("data/textures/ramlethal/RamuSword_albedo.png", 
                                          TYPE_TEXTURES::kAlbedo);
  vTextures.push_back(myMaterial->m_pTexture);

  myMaterial = myRSRCMG->load<Materials>("data/textures/ramlethal/RamuSword_metallic.png",
                                         TYPE_TEXTURES::kMetallic);
  vTextures.push_back(myMaterial->m_pTexture);

  myMaterial = myRSRCMG->load<Materials>("data/textures/ramlethal/RamuSword_roughness.png",
                                         TYPE_TEXTURES::kRoughness);
  vTextures.push_back(myMaterial->m_pTexture);

  myMaterial = myRSRCMG->load<Materials>("data/textures/ramlethal/RamuSword_normal.png",
                                         TYPE_TEXTURES::kNormal);
  vTextures.push_back(myMaterial->m_pTexture);

  myMaterial = myRSRCMG->load<Materials>("data/textures/ramlethal/RamuSword_emissive.png",
                                         TYPE_TEXTURES::kEmissive);
  vTextures.push_back(myMaterial->m_pTexture);

  myMaterial = myRSRCMG->load<Materials>("data/textures/ramlethal/RamuSword_ao.png",
                                         TYPE_TEXTURES::kAO);
  vTextures.push_back(myMaterial->m_pTexture);

  /*
  * M A T E R I A L
  * Z O N E
  * T O
  * M E S H E S
  */
  uint32 sizeMeshes = myModel->getSizeMeshes();
  uint32 sizeTextures = vTextures.size();
  for (uint32 i = 0; i < sizeMeshes; ++i) {
    for (uint32 j = 0; j < sizeTextures; ++j) {

      uint32 sizeMeshTex = myModel->getMesh(i).m_vTextures.size();
      if (sizeTextures == sizeMeshTex) {
        myModel->getMesh(i).m_vTextures[j] = vTextures[j];
      }
    }
  }

  /*
  * Z O N E
  */
  SPtr<StaticMesh> myStaticMesh = make_shared<StaticMesh>();
  myStaticMesh->m_pModel = myModel;

  //Creating the component
  SPtr<Component> newComponent(myStaticMesh);

  //Creating actor
  SPtr<Actor> actor(new Actor("Sword"));
  actor->setIsSelected(true);
  actor->setComponent(newComponent);

  //Adding the actor to node root
  mySceneGraph->createNewActor(actor, SPtr<SceneNode>(nullptr));
}

void
AppTest::createNodeStranger() {
  auto mySceneGraph = SceneGraph::instancePtr();
  auto myRSRCMG = ResourceManager::instancePtr();

  SPtr<Models> myModel = myRSRCMG->load<Models>("data/models/stranger/The Stranger.fbx");

  /*
  * Z O N E
  */
  SPtr<StaticMesh> myStaticMesh = make_shared<StaticMesh>();
  myStaticMesh->m_pModel = myModel;

  //Creating the component
  SPtr<Component> newComponent(myStaticMesh);

  //Creating actor
  SPtr<Actor> actor(new Actor("The Stranger"));
  actor->setIsSelected(true);
  actor->setComponent(newComponent);

  //Adding the actor to node root
  mySceneGraph->createNewActor(actor, SPtr<SceneNode>(nullptr));
}

void
AppTest::createNodesMarco() {
  auto mySceneGraph = SceneGraph::instancePtr();
  auto myRSRCMG = ResourceManager::instancePtr();

  SPtr<Models> myModel = myRSRCMG->load<Models>("data/models/basicModels/cube.fbx");

  SPtr<StaticMesh> myStaticMesh = make_shared<StaticMesh>();
  myStaticMesh->m_pModel = myModel;

  //Creating the component
  SPtr<Component> newComponent(myStaticMesh);

  //Creating actor
  SPtr<Actor> actor(new Actor("Box_v2"));
  actor->setIsSelected(true);
  actor->setComponent(newComponent);

  //Adding the actor to node root
  mySceneGraph->createNewActor(actor, SPtr<SceneNode>(nullptr));
}

void
AppTest::createNodesMarco2() {
  auto mySceneGraph = SceneGraph::instancePtr();
  auto myRSRCMG = ResourceManager::instancePtr();

  SPtr<Models> myModel = myRSRCMG->load<Models>("data/models/marco/BattleDroid.fbx");

  SPtr<StaticMesh> myStaticMesh = make_shared<StaticMesh>();
  myStaticMesh->m_pModel = myModel;

  //Creating the component
  SPtr<Component> newComponent(myStaticMesh);

  //Creating actor
  SPtr<Actor> actor(new Actor("BattleDroid"));
  actor->setIsSelected(true);
  actor->setComponent(newComponent);

  //Adding the actor to node root
  mySceneGraph->createNewActor(actor, SPtr<SceneNode>(nullptr));
}

void
AppTest::createScene() {
  auto mySceneGraph = SceneGraph::instancePtr();
  auto myRSRCMG = ResourceManager::instancePtr();

  SPtr<Models> myModel = myRSRCMG->load<Models>("data/models/basicModels/plano.fbx");

  SPtr<StaticMesh> myStaticMesh = make_shared<StaticMesh>();
  myStaticMesh->m_pModel = myModel;

  //Creating the component
  SPtr<Component> newComponent(myStaticMesh);

  //Creating actor
  SPtr<Actor> actor(new Actor("Plane"));
  actor->setIsSelected(true);
  actor->setComponent(newComponent);

  //Adding the actor to node root
  mySceneGraph->createNewActor(actor, SPtr<SceneNode>(nullptr));
}