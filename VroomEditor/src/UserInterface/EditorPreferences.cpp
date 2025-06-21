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
    if (ImGui::CollapsingHeader("Editor colors"))
    {
      _editColors();
    }
  }
  ImGui::End();

  return false;
}

void EditorPreferences::_editColors()
{
  auto& colors = ImGui::GetStyle().Colors;

  ImGui::ColorEdit4("Text", &colors[ImGuiCol_Text].x);
  ImGui::ColorEdit4("TextDisabled", &colors[ImGuiCol_TextDisabled].x);
  ImGui::ColorEdit4("WindowBg", &colors[ImGuiCol_WindowBg].x);
  ImGui::ColorEdit4("ChildBg", &colors[ImGuiCol_ChildBg].x);
  ImGui::ColorEdit4("PopupBg", &colors[ImGuiCol_PopupBg].x);
  ImGui::ColorEdit4("Border", &colors[ImGuiCol_Border].x);
  ImGui::ColorEdit4("BorderShadow", &colors[ImGuiCol_BorderShadow].x);
  ImGui::ColorEdit4("FrameBg", &colors[ImGuiCol_FrameBg].x);
  ImGui::ColorEdit4("FrameBgHovered", &colors[ImGuiCol_FrameBgHovered].x);
  ImGui::ColorEdit4("FrameBgActive", &colors[ImGuiCol_FrameBgActive].x);
  ImGui::ColorEdit4("TitleBg", &colors[ImGuiCol_TitleBg].x);
  ImGui::ColorEdit4("TitleBgActive", &colors[ImGuiCol_TitleBgActive].x);
  ImGui::ColorEdit4("TitleBgCollapsed", &colors[ImGuiCol_TitleBgCollapsed].x);
  ImGui::ColorEdit4("MenuBarBg", &colors[ImGuiCol_MenuBarBg].x);
  ImGui::ColorEdit4("ScrollbarBg", &colors[ImGuiCol_ScrollbarBg].x);
  ImGui::ColorEdit4("ScrollbarGrab", &colors[ImGuiCol_ScrollbarGrab].x);
  ImGui::ColorEdit4("ScrollbarGrabHovered", &colors[ImGuiCol_ScrollbarGrabHovered].x);
  ImGui::ColorEdit4("ScrollbarGrabActive", &colors[ImGuiCol_ScrollbarGrabActive].x);
  ImGui::ColorEdit4("CheckMark", &colors[ImGuiCol_CheckMark].x);
  ImGui::ColorEdit4("SliderGrab", &colors[ImGuiCol_SliderGrab].x);
  ImGui::ColorEdit4("SliderGrabActive", &colors[ImGuiCol_SliderGrabActive].x);
  ImGui::ColorEdit4("Button", &colors[ImGuiCol_Button].x);
  ImGui::ColorEdit4("ButtonHovered", &colors[ImGuiCol_ButtonHovered].x);
  ImGui::ColorEdit4("ButtonActive", &colors[ImGuiCol_ButtonActive].x);
  ImGui::ColorEdit4("Header", &colors[ImGuiCol_Header].x);
  ImGui::ColorEdit4("HeaderHovered", &colors[ImGuiCol_HeaderHovered].x);
  ImGui::ColorEdit4("HeaderActive", &colors[ImGuiCol_HeaderActive].x);
  ImGui::ColorEdit4("Separator", &colors[ImGuiCol_Separator].x);
  ImGui::ColorEdit4("SeparatorHovered", &colors[ImGuiCol_SeparatorHovered].x);
  ImGui::ColorEdit4("SeparatorActive", &colors[ImGuiCol_SeparatorActive].x);
  ImGui::ColorEdit4("ResizeGrip", &colors[ImGuiCol_ResizeGrip].x);
  ImGui::ColorEdit4("ResizeGripHovered", &colors[ImGuiCol_ResizeGripHovered].x);
  ImGui::ColorEdit4("ResizeGripActive", &colors[ImGuiCol_ResizeGripActive].x);
  ImGui::ColorEdit4("InputTextCursor", &colors[ImGuiCol_InputTextCursor].x);
  ImGui::ColorEdit4("TabHovered", &colors[ImGuiCol_TabHovered].x);
  ImGui::ColorEdit4("Tab", &colors[ImGuiCol_Tab].x);
  ImGui::ColorEdit4("TabSelected", &colors[ImGuiCol_TabSelected].x);
  ImGui::ColorEdit4("TabSelectedOverline", &colors[ImGuiCol_TabSelectedOverline].x);
  ImGui::ColorEdit4("TabDimmed", &colors[ImGuiCol_TabDimmed].x);
  ImGui::ColorEdit4("TabDimmedSelected", &colors[ImGuiCol_TabDimmedSelected].x);
  ImGui::ColorEdit4("TabDimmedSelectedOverline", &colors[ImGuiCol_TabDimmedSelectedOverline].x);
  ImGui::ColorEdit4("DockingPreview", &colors[ImGuiCol_DockingPreview].x);
  ImGui::ColorEdit4("DockingEmptyBg", &colors[ImGuiCol_DockingEmptyBg].x);
  ImGui::ColorEdit4("PlotLines", &colors[ImGuiCol_PlotLines].x);
  ImGui::ColorEdit4("PlotLinesHovered", &colors[ImGuiCol_PlotLinesHovered].x);
  ImGui::ColorEdit4("PlotHistogram", &colors[ImGuiCol_PlotHistogram].x);
  ImGui::ColorEdit4("PlotHistogramHovered", &colors[ImGuiCol_PlotHistogramHovered].x);
  ImGui::ColorEdit4("TableHeaderBg", &colors[ImGuiCol_TableHeaderBg].x);
  ImGui::ColorEdit4("TableBorderStrong", &colors[ImGuiCol_TableBorderStrong].x);
  ImGui::ColorEdit4("TableBorderLight", &colors[ImGuiCol_TableBorderLight].x);
  ImGui::ColorEdit4("TableRowBg", &colors[ImGuiCol_TableRowBg].x);
  ImGui::ColorEdit4("TableRowBgAlt", &colors[ImGuiCol_TableRowBgAlt].x);
  ImGui::ColorEdit4("TextLink", &colors[ImGuiCol_TextLink].x);
  ImGui::ColorEdit4("TextSelectedBg", &colors[ImGuiCol_TextSelectedBg].x);
  ImGui::ColorEdit4("TreeLines", &colors[ImGuiCol_TreeLines].x);
  ImGui::ColorEdit4("DragDropTarget", &colors[ImGuiCol_DragDropTarget].x);
  ImGui::ColorEdit4("NavCursor", &colors[ImGuiCol_NavCursor].x);
  ImGui::ColorEdit4("NavWindowingHighlight", &colors[ImGuiCol_NavWindowingHighlight].x);
  ImGui::ColorEdit4("NavWindowingDimBg", &colors[ImGuiCol_NavWindowingDimBg].x);
  ImGui::ColorEdit4("ModalWindowDimBg", &colors[ImGuiCol_ModalWindowDimBg].x);
}
