#include <imgui.h>
#include <gaModels.h>
#include <gaStaticMesh.h>
#include <gaGraphicsApi.h>
#include <imfilebrowser.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>
#include <gaBaseOmniConnect.h>
#include <gaRenderTarget.h>
#include <gaResourceManager.h>

#include "gaInterface.h"

/*
* ImGui notes
* io.WantCaptureMouse, if it is 0 it means that your pointer is
*                      outside a window and if it is 1 it is inverse of 0.
*/

namespace gaEngineSDK {
  void 
  Interface::init(uint32 width, uint32 height, void* hwndHandle) {
    auto myGraphicsApi = g_graphicApi().instancePtr();

    m_width = width;
    m_height = height;
    m_windowsHandle = hwndHandle;

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    ImGui_ImplWin32_Init(m_windowsHandle);
    ImGui_ImplDX11_Init((ID3D11Device*)myGraphicsApi->getDevice(), 
                        (ID3D11DeviceContext*)myGraphicsApi->getDeviceContext());
    ImGui::StyleColorsDark();

    //io.Fonts->AddFontFromFileTTF("data/misc/fonts/CoD.otf", 16.0f);

    /*
    */
    m_text.resize(500);
    m_size = 100.0f;
    /*
    */

    /*
    */
    m_omniFolderPath.resize(400);
    m_omniStageName.resize(400);
    /*
    */

    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
  }

  void 
  Interface::update(const float& deltaTime) {
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
  }

  void 
  Interface::render() {
    isMouseTouchingImGui();

    /*
    * C A L L S
    * F O R
    * I M G U I
    */
    //imguiDocking();
    imguiOptionsBar();
    imguiScenegraph();
    imguiModelsInfo();
    imguiCurrentFPS();
    omniverseWindow();

    //Function to know decompressImages is working
    //descompressImages();

    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
  }

  void
  Interface::textCentered(String text) {
    auto windowWidth = ImGui::GetWindowSize().x;
    auto textWidth = ImGui::CalcTextSize(text.c_str()).x;

    ImGui::SetCursorPosX((windowWidth - textWidth) * 0.5f);
    ImGui::Text(text.c_str());
  }

  void 
  Interface::textColoredCentered(String text, Vector3 rgb) {
    auto windowWidth = ImGui::GetWindowSize().x;
    auto textWidth = ImGui::CalcTextSize(text.c_str()).x;

    ImGui::SetCursorPosX((windowWidth - textWidth) * 0.5f);
    ImGui::TextColored(ImVec4(rgb.x, rgb.y, rgb.z, 1.0f), text.c_str());
  }

  void 
  Interface::materialsInterface() {
    auto mySceneGraph = SceneGraph::instancePtr();
    auto myGraphicsApi = g_graphicApi().instancePtr();

    auto object = mySceneGraph->m_nodeSelected->getActorNode().get();
    auto tempStaticMesh = object->getComponent<StaticMesh>();

    textColoredCentered("-Materials and Meshes-", Vector3(255.0f, 165.0f, 0.0f));
    ImGui::Separator();

    uint32 sizeMeshes = tempStaticMesh->m_pModel->getSizeMeshes();
    uint32 textureSize = 0;
    String nameMesh;
    String nameTexture;

    //Cycle for selected meshes
    for (uint32 i = 0; i < sizeMeshes; ++i) {
      textureSize = tempStaticMesh->m_pModel->getMesh(i)->m_vTextures.size();
      auto texturesInfo = tempStaticMesh->m_pModel->getMesh(i)->m_vTextures;

      if (0 != textureSize) {
        nameMesh = "Mesh - ";
        nameMesh += to_string(i);
        textColoredCentered(nameMesh, Vector3(255.0f, 165.0f, 0.0f));
      }

      for (uint32 j = 0; j < textureSize; ++j) {
        if (nullptr != texturesInfo.at(j)) {
          nameTexture = texturesInfo.at(j)->m_textureName;
          ImGui::Text(nameTexture.c_str());

          ImTextureID id = tempStaticMesh->m_pModel->getMesh(i)->m_vTextures[j]->getTexture();
          ImGui::Image(id, ImVec2(120, 120));
          ImGui::Separator();
        }
      }
      ImGui::Separator();
    }
  }

  void
  Interface::transformInterface() {
    auto mySceneGraph = SceneGraph::instancePtr();

    auto object = mySceneGraph->m_nodeSelected->getActorNode().get();

    textColoredCentered("-Transform-", Vector3(255.0f, 165.0f, 0.0f));
    ImGui::Separator();

    auto transform = object->getComponent<Transform>();

    auto position = transform->getPosition();
    auto rotation = transform->getEulerRotation();
    auto scale = transform->getScale();

    ImGui::DragFloat3("-Position-", &position.x);
    ImGui::Separator();
    ImGui::DragFloat3("-Rotation-", &rotation.x);
    ImGui::Separator();
    ImGui::DragFloat3("-Scale-", &scale.x);
    ImGui::Separator();

    if ((position != transform->getPosition()) || 
        (rotation != transform->getEulerRotation()) || 
        (scale != transform->getScale())) {

      //Sets
      transform->setPosition(position);
      transform->setEulerRotation(rotation);
      transform->setScale(scale);

      auto myOmniverse = g_baseOmniConnect().instancePtr();
      if (myOmniverse->getIsLiveSync()) {
        myOmniverse->updateGaToOmni();
      }
    }
  }

  /***************************************************************************/
  /**
  * Inputs for ImGui.
  */
  /***************************************************************************/

  void 
  Interface::isMouseTouchingImGui() {
    ImGuiIO& io = ImGui::GetIO();

    //No Touching
    if (0 == io.WantCaptureMouse) {
      m_touchingImGui = false;
    }
    else {
      m_touchingImGui = true;
    }
  }

  void 
  Interface::mouseButtonDown(uint32 button) {
    ImGuiIO& io = ImGui::GetIO();

    if (!(ImGui::IsAnyMouseDown()) && (nullptr == ::GetCapture())) {
      ::SetCapture((HWND)m_windowsHandle);
    }

    io.MouseDown[button] = true;
  }

  void
  Interface::mouseButtonReleased(uint32 button) {
    ImGuiIO& io = ImGui::GetIO();

    if (!(ImGui::IsAnyMouseDown()) && (m_windowsHandle == ::GetCapture())) {
      ::ReleaseCapture();
    }

    io.MouseDown[button] = false;
  }

  void 
  Interface::textEnter(wchar_t unicode) {
    ImGuiIO& io = ImGui::GetIO();

    io.AddInputCharacterUTF16((USHORT)unicode);
  }

  void 
  Interface::onKeyPressed(bool alt, bool ctrl, bool shift, bool sys, int32 code) {
    ImGuiIO& io = ImGui::GetIO();

    if (code >= 0) {
      io.KeysDown[static_cast<int32>(code)] = true;
    }
  }

  void 
  Interface::onKeyReleased(bool alt, bool ctrl, bool shift, bool sys, int32 code) {
    ImGuiIO& io = ImGui::GetIO();

    if (code >= 0) {
      io.KeysDown[static_cast<int32>(code)] = false;
    }
  }

  /***************************************************************************/
  /**
  * Methods for ImGui.
  */
  /***************************************************************************/

  void
  Interface::imguiDocking() {
    ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
  }

  void
  Interface::imguiOptionsBar() {
    if (ImGui::BeginMainMenuBar()) {
      if (ImGui::BeginMenu("File")) {
        if (ImGui::MenuItem("New", "CTRL+N")) {

        }
        if (ImGui::MenuItem("Open", "CTRL+O")) {
        }
        if (ImGui::MenuItem("Save", "CTRL+S")) {
        }
        if (ImGui::MenuItem("Save As", "CTRL+A")) {
        }
        ImGui::Separator();

        if (ImGui::MenuItem("Exit", "CTRL+E")) {
          exit(0);
        }
        ImGui::EndMenu();
      }
      if (ImGui::BeginMenu("Edit")) {
        if (ImGui::MenuItem("Reset Scene graph")) {
          auto mySceneGraph = SceneGraph::instancePtr();
          mySceneGraph->clearSceneGraph();
        }
        if (ImGui::MenuItem("Open POD")) {
          createNodePod();
        }
        if (ImGui::MenuItem("Open Vela")) {
          createNodeVela();
        }
        if (ImGui::MenuItem("Open 2B")) {
          createNode2B();
        }
        if (ImGui::MenuItem("Open Knuckles")) {
          createNodeKnucles();
        }
        if (ImGui::MenuItem("Open Grimoires")) {
          createNodeGrimoires();
        }
        if (ImGui::MenuItem("Open Ramlethal's swords")) {
          createNodeRamlethalSwords();
        }
        if (ImGui::MenuItem("Open Exo stranger")) {
          createNodeStranger();
        }
        if (ImGui::MenuItem("Open Plane")) {
          createNodePlane();
        }
        ImGui::EndMenu();
      }
      if (ImGui::BeginMenu("Window")) {
        if (ImGui::MenuItem("New Window")) {

        }
        ImGui::EndMenu();
      }
      if (ImGui::BeginMenu("Omniverse")) {
        auto myOmniverse = g_baseOmniConnect().instancePtr();

        if (!(myOmniverse->getIsStartUp())) {
          if (ImGui::Button("Start Omniverse")) {
            if (!(myOmniverse->startOmni())) {
              cout << "\nError, can not start Omniverse\n";
            }

            cout << "\nOmniverse Started\n";
            m_showOmniImGui = true;
          }
        }
        else {
          if (ImGui::Button("Stop Omniverse")) {
            cout << "\n\nOmniverse stopped\n\n";
            myOmniverse->shutdownOmni();
            m_showOmniImGui = false;
          }

          ImGui::Checkbox("Show Omniverse Window", &m_showOmniImGui);
        }
        ImGui::Separator();

        bool isOmniLog = myOmniverse->getOmniverseLogging();

        ImGui::Checkbox("Active Log (Debug)", &isOmniLog);

        myOmniverse->setOmniverseLog(isOmniLog);

        ImGui::EndMenu();
      }

      ImGui::EndMainMenuBar();
    }
  }

  void
  Interface::imguiScenegraph() {
    auto mySceneGraph = SceneGraph::instancePtr();

    ImGuiTreeNodeFlags myFlags = ImGuiWindowFlags_AlwaysVerticalScrollbar | 
                                 ImGuiWindowFlags_NoResize;

    if (ImGui::Begin("Scene graph", nullptr, myFlags)) {
      textColoredCentered("-Scene graph-", Vector3(255.0f, 165.0f, 0.0f));
      ImGui::Separator();

      imguiShowTreeNodes("nodeChild", mySceneGraph->m_root);

      ImGui::End();
    }
  }

  void
  Interface::imguiShowTreeNodes(String childName, WeakSPtr<SceneNode> childNode) {
    auto mySceneGraph = SceneGraph::instancePtr();
    auto node = childNode.lock();

    bool hasChildren = false;
    bool isOpen = false;

    ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_SpanFullWidth |
                               ImGuiTreeNodeFlags_OpenOnArrow |
                               ImGuiTreeNodeFlags_OpenOnDoubleClick;

    if (0 < node->getChildNodes().size()) {
      hasChildren = true;
    }

    if (nullptr == node->getParentNode().lock()) {
      flags |= ImGuiTreeNodeFlags_DefaultOpen;
      ImGui::SetNextItemOpen(true);
    }
    
    if (!(hasChildren)) {
      flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_Bullet;
    }

    //Obtain name node
    String actorName = node->getActorNode().get()->getActorName();
    isOpen = ImGui::TreeNodeEx(actorName.c_str(), flags);

    if (ImGui::IsItemClicked()) {
      node->getActorNode().get()->setIsSelected(true);

      if (node != mySceneGraph->m_nodeSelected) {
        if (nullptr != mySceneGraph->m_nodeSelected) {
          mySceneGraph->m_nodeSelected->m_isSelected = false;
        }

        mySceneGraph->m_nodeSelected = node;
      }
    }

    if (isOpen) {
      ImGui::AlignTextToFramePadding();

      if (0 < node->getChildNodes().size()) {
        for (auto childNode : node->getChildNodes()) {
          imguiShowTreeNodes(actorName, childNode);
        }
      }

      ImGui::TreePop();
    }
  }

  void 
  Interface::imguiModelsInfo() {
    auto mySceneGraph = SceneGraph::instancePtr();

    if (nullptr == mySceneGraph->m_nodeSelected) {
      return;
    }
    
    auto object = mySceneGraph->m_nodeSelected->getActorNode().get();

    ImGuiTreeNodeFlags myFlags = ImGuiWindowFlags_AlwaysVerticalScrollbar;

    if (ImGui::Begin("Model information", nullptr, myFlags)) {
      /*
      * S T A R T
      * I N F O
      */
      textColoredCentered("-Model information-", Vector3(255.0f, 165.0f, 0.0f));
      ImGui::Separator();

      /*
      * N A M E
      * M O D E L
      */
      String nameObj = object->getActorName();
      textColoredCentered("-Name-", Vector3(255.0f, 165.0f, 0.0f));
      textCentered(nameObj);
      ImGui::Separator();

      /*
      * T R A N S F O R M
      * M A T R I X
      * I N F O
      */
      transformInterface();

      /*
      * M A T E R I A L
      * Z O N E
      */
      materialsInterface();

      
      ImGui::End();
    }
  }

  void
  Interface::imguiCurrentFPS() {
    ImGuiTreeNodeFlags myFlags = ImGuiWindowFlags_NoTitleBar |
                                 ImGuiWindowFlags_NoBackground | 
                                 ImGuiWindowFlags_AlwaysVerticalScrollbar |
                                 ImGuiWindowFlags_NoScrollbar;

    if (ImGui::Begin("Frames Per Second", nullptr, myFlags)) {
      ImGuiIO& io = ImGui::GetIO();
      float fps = (1.0f / io.DeltaTime);

      //ImGui::Text("-Frames Per Second - - ", fps);
      ImGui::Text("%.3f - ms", (1000.0f / ImGui::GetIO().Framerate));
      ImGui::Separator();

      ImGui::Text("%.1f - FPS", ImGui::GetIO().Framerate);
      ImGui::Separator();

      ImGui::Text("%.1f - 1.0 and DeltaTime", fps);
      ImGui::Separator();

      ImGui::End();
    }
  }

  void 
  Interface::omniverseWindow() {
    if (!(m_showOmniImGui)) {
      return;
    }

    auto myOmniverse = g_baseOmniConnect().instancePtr();

    if (ImGui::Begin("Omniverse", &m_showOmniImGui)) {
      ImGui::InputText("Folder Path", m_omniFolderPath.data(), 400);
      ImGui::InputText("Stage Name", m_omniStageName.data(), 400);
      
      if (ImGui::Button("Default local path")) {
        m_omniFolderPath = "http://localhost:8080/omniverse://127.0.0.1/Users/";
        m_omniFolderPath.resize(400);
      }
      ImGui::Separator();

      if (ImGui::Button("Create Stage (Sub meshes)")) {
        const String stageUrl = myOmniverse->createOmniScene(m_omniFolderPath,
                                                             m_omniStageName);
        if (stageUrl.empty()) {
          cout << "\nError, can not create Omniverse scene\n";
        }
        else {
          //Print the username for the server
          myOmniverse->connectedUsername(m_omniFolderPath + m_omniStageName);

          myOmniverse->saveSGToUSD();
        }
      }
      //ImGui::SameLine();
      //if (ImGui::Button("Create Stage (Meshes)")) {
      //  const String stageUrl = myOmniverse->createOmniScene(m_omniFolderPath,
      //                                                       m_omniStageName);
      //  if (stageUrl.empty()) {
      //    cout << "\nError, can not create Omniverse scene\n";
      //  }
      //  else {
      //    //Print the username for the server
      //    myOmniverse->connectedUsername(m_omniFolderPath + m_omniStageName);
      //  
      //    myOmniverse->saveSGToUSD();
      //  }
      //}

      ImGui::Separator();
      if (ImGui::Button("Open Stage")) {
        if (!(myOmniverse->openUSD(m_omniFolderPath, m_omniStageName))) {
          cout << "\nError, can not open the USD\n";
        }
      }
      ImGui::SameLine();
      if (ImGui::Button("Get Scene")) {
        myOmniverse->loadUSD();
      }
      ImGui::Separator();

      /*
      */
      bool tempLiveSync = myOmniverse->getIsLiveSync();
      bool isOmniLog = myOmniverse->getOmniverseLogging();

      ImGui::Checkbox("Active Live Sync", &tempLiveSync);
      ImGui::Checkbox("Active Log (Debug)", &isOmniLog);


      myOmniverse->setIsLiveSync(tempLiveSync);
      myOmniverse->setOmniverseLog(isOmniLog);
      /*
      */

      ImGui::End();
    }
  }

  void
  Interface::createNodePod() {
    auto mySceneGraph = SceneGraph::instancePtr();
    auto myRSRCMG = ResourceManager::instancePtr();
  
    SPtr<Models> myModel = myRSRCMG->load<Models>("data/models/pod/Pod.fbx");
  
    SPtr<StaticMesh> myStaticMesh = make_shared<StaticMesh>();
    myStaticMesh->m_pModel = myModel;
  
    //Creating the component
    SPtr<Component> newComponent(myStaticMesh);
  
    //Creating actor
    SPtr<Actor> actor = make_shared<Actor>();
    actor->init("Pod");
    actor->setIsSelected(true);
    actor->setComponent(newComponent);
  
    //Adding the actor to node root
    mySceneGraph->createNewActor(actor, SPtr<SceneNode>(nullptr));
  }
  
  void
  Interface::createNodeVela() {
    auto mySceneGraph = SceneGraph::instancePtr();
    auto myRSRCMG = ResourceManager::instancePtr();
  
    SPtr<Models> myModel = myRSRCMG->load<Models>("data/models/vela/Vela2.fbx");
  
    SPtr<StaticMesh> myStaticMesh = make_shared<StaticMesh>();
    myStaticMesh->m_pModel = myModel;
  
    //Creating the component
    SPtr<Component> newComponent(myStaticMesh);
  
    //Creating actor
    SPtr<Actor> actor = make_shared<Actor>();
    actor->init("Vela");
    actor->setIsSelected(true);
    actor->setComponent(newComponent);
  
    //Adding the actor to node root
    mySceneGraph->createNewActor(actor, SPtr<SceneNode>(nullptr));
  }
  
  void
  Interface::createNode2B() {
    auto mySceneGraph = SceneGraph::instancePtr();
  
    SPtr<Models> myModel = make_shared<Models>();
    myModel->loadFromFile("data/models/2B/2B.obj");
  
    SPtr<StaticMesh> myStaticMesh = make_shared<StaticMesh>();
    myStaticMesh->m_pModel = myModel;
  
    //Creating the component
    SPtr<Component> newComponent(myStaticMesh);
  
    //Creating actor
    SPtr<Actor> actor = make_shared<Actor>();
    actor->init("2B");
    actor->setIsSelected(true);
    actor->setComponent(newComponent);
  
    //Adding the actor to node root
    mySceneGraph->createNewActor(actor, SPtr<SceneNode>(nullptr));
  }
  
  void
  Interface::createNodeKnucles() {
    auto mySceneGraph = SceneGraph::instancePtr();
    auto myRSRCMG = ResourceManager::instancePtr();
  
    SPtr<Models> myModel = myRSRCMG->load<Models>("data/models/ugandan/Knuckles.fbx");
  
    SPtr<StaticMesh> myStaticMesh = make_shared<StaticMesh>();
    myStaticMesh->m_pModel = myModel;
  
    //Creating the component
    SPtr<Component> newComponent(myStaticMesh);
  
    //Creating actor
    SPtr<Actor> actor = make_shared<Actor>();
    actor->init("Ugandan");
    actor->setIsSelected(true);
    actor->setComponent(newComponent);
  
    //Adding the actor to node root
    mySceneGraph->createNewActor(actor, SPtr<SceneNode>(nullptr));
  }
  
  void 
  Interface::createNodeGrimoires() {
    auto mySceneGraph = SceneGraph::instancePtr();
    auto myRSRCMG = ResourceManager::instancePtr();
  
    SPtr<Models> myModel = myRSRCMG->load<Models>("data/models/grimoires/grimoires.fbx");
  
    SPtr<StaticMesh> myStaticMesh = make_shared<StaticMesh>();
    myStaticMesh->m_pModel = myModel;
  
    //Creating the component
    SPtr<Component> newComponent(myStaticMesh);
  
    //Creating actor
    SPtr<Actor> actor = make_shared<Actor>();
    actor->init("Grimoires");
    actor->setIsSelected(true);
    actor->setComponent(newComponent);
  
    //Adding the actor to node root
    mySceneGraph->createNewActor(actor, SPtr<SceneNode>(nullptr));
  }
  
  void 
  Interface::createNodeRamlethalSwords() {
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
  
        uint32 sizeMeshTex = myModel->getMesh(i)->m_vTextures.size();
        if (sizeTextures == sizeMeshTex) {
          myModel->getMesh(i)->m_vTextures[j] = vTextures[j];
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
    SPtr<Actor> actor = make_shared<Actor>();
    actor->init("Sword");
    actor->setIsSelected(true);
    actor->setComponent(newComponent);
  
    //Adding the actor to node root
    mySceneGraph->createNewActor(actor, SPtr<SceneNode>(nullptr));
  }
  
  void
  Interface::createNodeStranger() {
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
    SPtr<Actor> actor = make_shared<Actor>();
    actor->init("Stranger");
    actor->setIsSelected(true);
    actor->setComponent(newComponent);
  
    //Adding the actor to node root
    mySceneGraph->createNewActor(actor, SPtr<SceneNode>(nullptr));
  }
  
  void
  Interface::createNodePlane() {
    auto mySceneGraph = SceneGraph::instancePtr();
    auto myRSRCMG = ResourceManager::instancePtr();
  
    SPtr<Models> myModel = myRSRCMG->load<Models>("data/models/basicModels/plano.fbx");
  
    SPtr<StaticMesh> myStaticMesh = make_shared<StaticMesh>();
    myStaticMesh->m_pModel = myModel;
  
    //Creating the component
    SPtr<Component> newComponent(myStaticMesh);
  
    //Creating actor
    SPtr<Actor> actor = make_shared<Actor>();
    actor->init("Plane");
    actor->setIsSelected(true);
    actor->setComponent(newComponent);
  
    //Adding the actor to node root
    mySceneGraph->createNewActor(actor, SPtr<SceneNode>(nullptr));
  }

  /*
  * D O - N O T
  * T O
  * U S E
  */

  bool isFirtsTime = true;
  Vector<SPtr<Textures>> vTextures;
  
  void
  Interface::descompressImages() {
    ImGuiTreeNodeFlags myFlags = ImGuiWindowFlags_NoTitleBar |
                                 ImGuiWindowFlags_AlwaysVerticalScrollbar;
  
    if (ImGui::Begin("Des compress Images", nullptr, myFlags)) {
      /*
      * S T A R T
      * I N F O
      */
      textColoredCentered("-Des compress-", Vector3(255.0f, 165.0f, 0.0f));
      ImGui::Separator();
  
      auto myGraphicsApi = g_graphicApi().instancePtr();
      String nameTexture;
  
      if (isFirtsTime) {
        //vTextures = myGraphicsApi->loadCompressedTexture("data/textures/marco/testCompress.png");
        vTextures = myGraphicsApi->loadCompressedTexture("data/textures/marco/pruebame.gat");
        isFirtsTime = false;
      }
  
      uint32 sizeTextures = vTextures.size();
      for (uint32 i = 0; i < sizeTextures; ++i) {
        if (nullptr != vTextures.at(i)) {
          nameTexture = vTextures.at(i)->m_textureName;
          ImGui::Text(nameTexture.c_str());
  
          ImTextureID id = vTextures[i]->getTexture();
          ImGui::Image(id, ImVec2(120, 120));
          ImGui::Separator();
        }
      }
  
      ImGui::End();
    }
  }
}