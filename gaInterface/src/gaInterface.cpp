#include <gaGraphicsApi.h>

#include "gaInterface.h"

#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"

/*
widgets 548 - 429
imgui 429 - 86
imgui draw 86 - 3
imgui impl win 32 3 - 
*/

namespace gaEngineSDK {
  void 
  Interface::init(uint32 width, uint32 height, void* hwndHandle) {
    auto myGraphicsApi = g_graphicApi().instancePtr();

    m_width = width;
    m_height = height;
    m_hwndHandle = hwndHandle;

    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO();

    ImGui::StyleColorsDark();
  }

  void 
  Interface::update(const float& deltaTime) {
    ImGui::NewFrame();
  }

  void 
  Interface::render() {
    ImGui::Begin("Test window");
    ImGui::End();

    ImGui::Render();
  }
}