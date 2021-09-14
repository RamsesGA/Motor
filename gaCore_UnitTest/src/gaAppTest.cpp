#include <Windows.h>
#include <gaPlane.h>
#include <gaModels.h>
#include <gaTextures.h>
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
  
  //createCube();
  //createNodePod();
  //createNodeVela();
  //createNodeTwoB();
  //createNodeSpartan();
  //createNodeUgandan();
  createNodeGrimoires();
  //createNodeRamlethalSwords();
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

  /*
  * M A T E R I A L
  * Z O N E
  */
  Vector<Textures*> vTextures;
  SPtr<Materials> myMaterial = myRSRCMG->load<Materials>
                                         ("data/textures/spartan/Spartan_Chest_Mat_BaseColor.png", 
                                          TYPE_TEXTURES::kAlbedo);
  vTextures.push_back(myMaterial->m_pTexture);

  myMaterial = myRSRCMG->load<Materials>("data/textures/spartan/Spartan_Helmet_Mat_Metallic.png", 
                                         TYPE_TEXTURES::kMetallic);
  vTextures.push_back(myMaterial->m_pTexture);

  myMaterial = myRSRCMG->load<Materials>("data/textures/spartan/Spartan_Helmet_Mat_Roughness.png", 
                                         TYPE_TEXTURES::kRoughness);
  vTextures.push_back(myMaterial->m_pTexture);

  myMaterial = myRSRCMG->load<Materials>("data/textures/spartan/Spartan_Helmet_Mat_Normal.png", 
                                         TYPE_TEXTURES::kNormal);
  vTextures.push_back(myMaterial->m_pTexture);

  /*myMaterial = myRSRCMG->load<Materials>("data/textures/spartan/.png", 
                                         TYPE_TEXTURES::kEmissive);*/

  myMaterial = myRSRCMG->load<Materials>("data/textures/spartan/Spartan_Helmet_Mat_AO.png", 
                                         TYPE_TEXTURES::kAO);
  vTextures.push_back(myMaterial->m_pTexture);



  /*
  * Z O N E
  */
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
    /*myModel->m_mMaterials["03 - Default"]->m_vTextures[i] = vTextures[i];
    myModel->m_mMaterials["02 - Default"]->m_vTextures[i] = vTextures[i];
    myModel->m_mMaterials["08 - Default"]->m_vTextures[i] = vTextures[i];
    myModel->m_mMaterials["09 - Default"]->m_vTextures[i] = vTextures[i];
    myModel->m_mMaterials["13 - Default"]->m_vTextures[i] = vTextures[i];
    myModel->m_mMaterials["07 - Default"]->m_vTextures[i] = vTextures[i];*/
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
AppTest::createCube() {
  auto mySceneGraph = SceneGraph::instancePtr();
  auto myRSRCMG = ResourceManager::instancePtr();

  SPtr<Models> myModel = myRSRCMG->load<Models>("data/models/cube/cube.obj");

  SPtr<StaticMesh> myStaticMesh = make_shared<StaticMesh>();
  myStaticMesh->m_pModel = myModel;

  //Creating the component
  SPtr<Component> newComponent(myStaticMesh);

  //Creating actor
  SPtr<Actor> actor(new Actor("Cube"));
  actor->setIsSelected(true);
  actor->setComponent(newComponent);

  //Adding the actor to node root
  mySceneGraph->createNewActor(actor, SPtr<SceneNode>(nullptr));
}