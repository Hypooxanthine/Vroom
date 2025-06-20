#include "VroomEditor/UserInterface/EditorPreferences.h"

#include "imgui.h"

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

  }
  ImGui::End();

  return true;
}
