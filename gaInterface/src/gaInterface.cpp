#include <gaGraphicsApi.h>
#include <imgui.h>

#include "gaInterface.h"

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