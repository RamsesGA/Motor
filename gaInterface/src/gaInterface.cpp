#include <gaGraphicsApi.h>
#include <imgui.h>
#include <imfilebrowser.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx11.h>
#include <gaStaticMesh.h>
#include <gaModels.h>
#include <gaBaseOmniConnect.h>

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

    io.Fonts->AddFontFromFileTTF("data/misc/fonts/CoD.otf", 16.0f);

    m_text.resize(500);
    m_size = 100.0f;
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
    imguiFile();
    imguiScenegraph();
    imguiModelsInfo();
    imguiCurrentFPS();

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
      textureSize = tempStaticMesh->m_pModel->getMesh(i).m_vTextures.size();
      auto texturesInfo = tempStaticMesh->m_pModel->getMesh(i).m_vTextures;

      if (0 != textureSize) {
        nameMesh = "Mesh - ";
        nameMesh += to_string(i);
        textColoredCentered(nameMesh, Vector3(255.0f, 165.0f, 0.0f));
      }

      for (uint32 j = 0; j < textureSize; ++j) {
        if (nullptr != texturesInfo.at(j)) {
          nameTexture = texturesInfo.at(j)->m_textureName;
          ImGui::Text(nameTexture.c_str());

          ImTextureID id = tempStaticMesh->m_pModel->getMesh(i).m_vTextures[j]->getTexture();
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

    //Sets
    transform->setPosition(position);
    transform->setEulerRotation(rotation);
    transform->setScale(scale);
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

  /***************************************************************************/
  /**
  * Methods for ImGui.
  */
  /***************************************************************************/

  void
  Interface::imguiFile() {
    auto myOmniverse = g_baseOmniConnect().instancePtr();
    if (ImGui::BeginMainMenuBar()) {
      if (ImGui::BeginMenu("File")) {
        if (ImGui::MenuItem("New", "CTRL+N")) {

        }
        if (ImGui::MenuItem("Open", "CTRL+O")) {
          openFiles();
        }
        if (ImGui::MenuItem("Save", "CTRL+S")) {

        }
        if (ImGui::MenuItem("Save Ass", "CTRL+A")) {

        }
        ImGui::Separator();

        if (ImGui::MenuItem("Exit", "CTRL+E")) {
          exit(0);
        }
        ImGui::EndMenu();
      }
      if (ImGui::BeginMenu("Edit")) {
        if (ImGui::MenuItem("Cut", "CTRL+X")) {

        }
        if (ImGui::MenuItem("Copy", "CTRL+C")) {

        }
        if (ImGui::MenuItem("Paste", "CTRL+V")) {

        }
        if (ImGui::MenuItem("Delete", "DEL")) {

        }
        ImGui::EndMenu();
      }
      if (ImGui::BeginMenu("Window")) {
        if (ImGui::MenuItem("New Window")) {

        }
        ImGui::EndMenu();
      }

      ImGui::EndMainMenuBar();
    }
  }

  void
  Interface::imguiScenegraph() {
    auto mySceneGraph = SceneGraph::instancePtr();

    ImGuiTreeNodeFlags myFlags = ImGuiWindowFlags_NoTitleBar |
                                 ImGuiWindowFlags_AlwaysVerticalScrollbar |
                                 ImGuiWindowFlags_NoMove |
                                 ImGuiWindowFlags_NoResize;

    if (ImGui::Begin("Scene graph", nullptr, myFlags)) {
      textColoredCentered("-Scene graph-", Vector3(255.0f, 165.0f, 0.0f));
      ImGui::Separator();

      imguiShowTreeNodes("nodeChild", mySceneGraph->m_root);
    }
    ImGui::End();
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
      ImGui::SetNextTreeNodeOpen(true);
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

    ImGuiTreeNodeFlags myFlags = ImGuiWindowFlags_NoTitleBar |
                                 ImGuiWindowFlags_AlwaysVerticalScrollbar |
                                 ImGuiWindowFlags_NoMove |
                                 ImGuiWindowFlags_NoResize;

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
                                 ImGuiWindowFlags_NoMove | 
                                 ImGuiWindowFlags_NoResize | 
                                 ImGuiWindowFlags_NoBackground | 
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
  Interface::openFiles() {
    //myOmniverse->openUSDFiles("http://localhost:8080/omniverse://127.0.0.1/Users/gaEngine/test.usd");
  }

}