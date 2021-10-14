#include <gaGraphicsApi.h>
#include <imgui.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx11.h>

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

    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
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
    if (ImGui::BeginMainMenuBar()) {
      if (ImGui::BeginMenu("File")) {
        if (ImGui::MenuItem("New", "CTRL+N")) {

        }
        if (ImGui::MenuItem("Open", "CTRL+O")) {

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

    if (ImGui::Begin("Scene graph", nullptr, ImGuiWindowFlags_NoTitleBar |
                                             ImGuiWindowFlags_AlwaysVerticalScrollbar |
                                             ImGuiWindowFlags_NoMove)) {
      ImGui::TextColored(ImVec4(255.0f, 165.0f, 0.0f, 1.0f), "--Scene graph--");
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

    if (ImGui::Begin("Model information", nullptr, ImGuiWindowFlags_NoTitleBar |
                                                   ImGuiWindowFlags_AlwaysVerticalScrollbar |
                                                   ImGuiWindowFlags_NoMove)) {
      ImGui::TextColored(ImVec4(255.0f, 165.0f, 0.0f, 1.0f), "--Model information--");
      ImGui::Separator();

      ImGui::Text("Transform");
      ImGui::Separator();

      auto transform = object->getComponent<Transform>();

      auto position = transform->getPosition();
      auto rotation = transform->getEulerRotation();
      auto scale = transform->getScale();

      ImGui::DragFloat3("Position", &position.x);
      ImGui::Separator();
      ImGui::DragFloat3("Rotation", &rotation.x);
      ImGui::Separator();
      ImGui::DragFloat3("Scale", &scale.x);
      ImGui::Separator();

      //Sets
      transform->setPosition(position);
      transform->setEulerRotation(rotation);
      transform->setScale(scale);

      ImGui::Text("Materials");
      ImGui::Separator();

      ImGui::Text("Base Color / Albedo / Diffuse");
      //ImTextureID temp = ;
      //ImGui::Image();
      ImGui::Separator();

      ImGui::Text("Normals");
      ImGui::Separator();

      ImGui::Text("Roughness");
      ImGui::Separator();

      ImGui::Text("Ambient Occlusion");
      ImGui::Separator();

      ImGui::Text("Metallic");
      ImGui::Separator();

      ImGui::Text("Emissive");
      ImGui::Separator();

      ImGui::End();
    }
  }
}