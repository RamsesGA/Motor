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
    m_hwndHandle = hwndHandle;

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    ImGui_ImplWin32_Init(m_hwndHandle);
    ImGui_ImplDX11_Init((ID3D11Device*)myGraphicsApi->getDevice(), 
                        (ID3D11DeviceContext*)myGraphicsApi->getDeviceContext());
    ImGui::StyleColorsDark();

    io.Fonts->AddFontFromFileTTF("data/misc/fonts/CoD.otf", 16.0f);

  }

  void 
  Interface::update(const float& deltaTime) {
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
  }

  void 
  Interface::render() {
    static bool checkBox1 = false;
    ImGuiIO& io = ImGui::GetIO();

    //No Touching
    if (io.WantCaptureMouse == 0) {
      m_touchingImGui = false;
    }
    else {
      m_touchingImGui = true;
    }

    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImVec2(m_width, 22));
    if (ImGui::Begin("Menu bar", nullptr, ImGuiWindowFlags_NoResize | 
                                          ImGuiWindowFlags_NoCollapse | 
                                          ImGuiWindowFlags_NoMove | 
                                          ImGuiWindowFlags_NoBringToFrontOnFocus | 
                                          ImGuiWindowFlags_NoTitleBar | 
                                          ImGuiWindowFlags_MenuBar |
                                          ImGuiWindowFlags_NoScrollWithMouse)) {
      if (ImGui::BeginMenuBar()) {
        const char* tabs[] = { "File", "Edit", "Window", "Help" };
        static int32 page = 0;

        for (int32 i = 0; i < IM_ARRAYSIZE(tabs); ++i) {
          int32 distance = i == page ? 0 : i > page ? i - page : page - i;

          ImGui::GetStyle().Colors[ImGuiCol_Button] = ImVec4(.4f, .4f, .55f, .8f);
          ImGui::GetStyle().Colors[ImGuiCol_MenuBarBg] = ImVec4(.488f, .488f, .63f, .8f);
          ImGui::GetStyle().ItemInnerSpacing = ImVec2(0.f, 4.f);
          ImGui::GetStyle().ItemSpacing = ImVec2(0.f, 4.f);

          if (ImGui::Button(tabs[i], ImVec2(ImGui::GetWindowSize().x / 
                                     IM_ARRAYSIZE(tabs) - 9, 0))) {
            page = i;
          }

          if (i < IM_ARRAYSIZE(tabs) - 1) {
            ImGui::SameLine();
          }
        }
        ImGui::EndMenuBar();
      }

    }
    ImGui::End();
    




    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
  }
}