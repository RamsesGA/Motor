#include <Windows.h>
#include <gaPlane.h>
#include <gaModels.h>
#include <gaMaterials.h>
#include <gaSceneGraph.h>
#include <gaStaticMesh.h>
#include <gaResourceManager.h>

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
  auto myGraphicsApi = g_graphicApi().instancePtr();

  //We send the window to the API.
  myGraphicsApi->initDevice(m_sfmlWindow.getSystemHandle());

  /***************************************************************************/
  /*
  * Loading models.
  * Set Node
  */
  /***************************************************************************/

  //createNodePod();
  createNodeVela();
  //createNodeTwoB();
  //createNodeSpartan();
  //createNodeUgandan();
  //createNodeGrimoires();
  //createNodeRamlethalSwords();
}

void
AppTest::onDestroySystem() { }

/*****************************************************************************/
/*
* Nodes.
*/
/*****************************************************************************/
void
AppTest::createNodePod() {
  auto mySceneGraph = SceneGraph::instancePtr();
  auto myRSRCMG = ResourceManager::instancePtr();

  SPtr<Models> myModel = myRSRCMG->load<Models>("data/models/pod/POD.obj");

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
AppTest::createNodeSpartan() {
  auto mySceneGraph = SceneGraph::instancePtr();
  auto myRSRCMG = ResourceManager::instancePtr();

  SPtr<Models> myModel = myRSRCMG->load<Models>("data/models/spartan/Spartan.fbx");

  SPtr<StaticMesh> myStaticMesh = make_shared<StaticMesh>();
  myStaticMesh->m_pModel = myModel;

  //Creating the component
  SPtr<Component> newComponent(myStaticMesh);

  //Creating actor
  SPtr<Actor> actor(new Actor("Spartan"));
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

  SPtr<Materials> myMaterial = myRSRCMG->load<Materials>
                                         ("data/textures/ramlethal/RamuSword_albedo.png", 
                                         TYPE_TEXTURES::kAlbedo);
  myModel->m_mMaterials["02 - Default"] = myMaterial;

  myMaterial = myRSRCMG->load<Materials>("data/textures/ramlethal/RamuSword_ao.png",
                                        TYPE_TEXTURES::kAO);
  myModel->m_mMaterials["03 - Default"] = myMaterial;

  myMaterial = myRSRCMG->load<Materials>("data/textures/ramlethal/RamuSword_emissive.png",
                                        TYPE_TEXTURES::kEmissive);
  myModel->m_mMaterials["07 - Default"] = myMaterial;

  myMaterial = myRSRCMG->load<Materials>("data/textures/ramlethal/RamuSword_metallic.png",
                                        TYPE_TEXTURES::kMetallic);
  myModel->m_mMaterials["08 - Default"] = myMaterial;

  myMaterial = myRSRCMG->load<Materials>("data/textures/ramlethal/RamuSword_normal.png",
                                        TYPE_TEXTURES::kNormal);
  myModel->m_mMaterials["09 - Default"] = myMaterial;

  myMaterial = myRSRCMG->load<Materials>("data/textures/ramlethal/RamuSword_roughness.png",
                                        TYPE_TEXTURES::kRoughness);
  myModel->m_mMaterials["13 - Default"] = myMaterial;

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