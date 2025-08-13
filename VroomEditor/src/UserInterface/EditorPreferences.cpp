#include "VroomEditor/UserInterface/EditorPreferences.h"

using namespace vrm;

EditorPreferences::EditorPreferences()
{

}

EditorPreferences::~EditorPreferences()
{

}

void EditorPreferences::onImgui()
{
  if (ImGui::Begin("Preferences", m_open))
  {
    if (ImGui::BeginTabBar("TabBar"))
    {
      if (ImGui::BeginTabItem("Appearance"))
      {
        _editImguiStyle();

        ImGui::EndTabItem();
      }

      ImGui::EndTabBar();
    }
  }
  ImGui::End();
}

void EditorPreferences::_editImguiStyle()
{
  ImGui::ShowStyleEditor(&m_imguiStyle);
}
