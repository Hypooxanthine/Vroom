#include "VroomEditor/UserInterface/EditorPreferences.h"

#include <array>
#include <map>

using namespace vrm;

EditorPreferences::EditorPreferences()
{

}

EditorPreferences::~EditorPreferences()
{

}

bool EditorPreferences::onImgui()
{
  if (ImGui::Begin("Editor preferences", m_open))
  {
    if (ImGui::BeginTabBar("TabBar"))
    {
      if (ImGui::BeginTabItem("Imgui appearance"))
      {
        _editImguiStyle();

        ImGui::EndTabItem();
      }

      ImGui::EndTabBar();
    }
  }
  ImGui::End();

  return false;
}

void EditorPreferences::_editImguiStyle()
{
  ImGui::ShowStyleEditor(&m_imguiStyle);
}
