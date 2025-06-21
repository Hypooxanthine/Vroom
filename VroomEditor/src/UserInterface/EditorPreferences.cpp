#include "VroomEditor/UserInterface/EditorPreferences.h"

#include <array>
#include <map>

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
    if (ImGui::BeginTabBar("TabBar"))
    {
      if (ImGui::BeginTabItem("Styles"))
      {
        if (ImGui::CollapsingHeader("Colors"))
        {
          _editColors();
        }
        if (ImGui::CollapsingHeader("Others"))
        {
          _editStyleNoColor();
        }

        ImGui::EndTabItem();
      }

      ImGui::EndTabBar();
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

void EditorPreferences::_editStyleNoColor()
{
  auto& style = ImGui::GetStyle();
  ImGuiStyle defaultStyle;
  ImGui::PushID("Alpha"); if (ImGui::Button("Reset")) style.Alpha = defaultStyle.Alpha; ImGui::SameLine(); ImGui::DragScalarN("Alpha", ImGuiDataType_Float, &style.Alpha, sizeof(style.Alpha) / sizeof(float)); ImGui::PopID();
  ImGui::PushID("DisabledAlpha"); if (ImGui::Button("Reset")) style.DisabledAlpha = defaultStyle.DisabledAlpha; ImGui::SameLine(); ImGui::DragScalarN("DisabledAlpha", ImGuiDataType_Float, &style.DisabledAlpha, sizeof(style.DisabledAlpha) / sizeof(float)); ImGui::PopID();
  ImGui::PushID("WindowPadding"); if (ImGui::Button("Reset")) style.WindowPadding = defaultStyle.WindowPadding; ImGui::SameLine(); ImGui::DragScalarN("WindowPadding", ImGuiDataType_Float, &style.WindowPadding, sizeof(style.WindowPadding) / sizeof(float)); ImGui::PopID();
  ImGui::PushID("WindowRounding"); if (ImGui::Button("Reset")) style.WindowRounding = defaultStyle.WindowRounding; ImGui::SameLine(); ImGui::DragScalarN("WindowRounding", ImGuiDataType_Float, &style.WindowRounding, sizeof(style.WindowRounding) / sizeof(float)); ImGui::PopID();
  ImGui::PushID("WindowBorderSize"); if (ImGui::Button("Reset")) style.WindowBorderSize = defaultStyle.WindowBorderSize; ImGui::SameLine(); ImGui::DragScalarN("WindowBorderSize", ImGuiDataType_Float, &style.WindowBorderSize, sizeof(style.WindowBorderSize) / sizeof(float)); ImGui::PopID();
  ImGui::PushID("WindowBorderHoverPadding"); if (ImGui::Button("Reset")) style.WindowBorderHoverPadding = defaultStyle.WindowBorderHoverPadding; ImGui::SameLine(); ImGui::DragScalarN("WindowBorderHoverPadding", ImGuiDataType_Float, &style.WindowBorderHoverPadding, sizeof(style.WindowBorderHoverPadding) / sizeof(float)); ImGui::PopID();
  ImGui::PushID("WindowMinSize"); if (ImGui::Button("Reset")) style.WindowMinSize = defaultStyle.WindowMinSize; ImGui::SameLine(); ImGui::DragScalarN("WindowMinSize", ImGuiDataType_Float, &style.WindowMinSize, sizeof(style.WindowMinSize) / sizeof(float)); ImGui::PopID();
  ImGui::PushID("WindowTitleAlign"); if (ImGui::Button("Reset")) style.WindowTitleAlign = defaultStyle.WindowTitleAlign; ImGui::SameLine(); ImGui::DragScalarN("WindowTitleAlign", ImGuiDataType_Float, &style.WindowTitleAlign, sizeof(style.WindowTitleAlign) / sizeof(float)); ImGui::PopID();
  // ImGui::DragScalarN("WindowMenuButtonPosition", ImGuiDataType_Float, &style.WindowMenuButtonPosition, sizeof(style.WindowMenuButtonPosition) / sizeof(float));
  ImGui::PushID("ChildRounding"); if (ImGui::Button("Reset")) style.ChildRounding = defaultStyle.ChildRounding; ImGui::SameLine(); ImGui::DragScalarN("ChildRounding", ImGuiDataType_Float, &style.ChildRounding, sizeof(style.ChildRounding) / sizeof(float)); ImGui::PopID();
  ImGui::PushID("ChildBorderSize"); if (ImGui::Button("Reset")) style.ChildBorderSize = defaultStyle.ChildBorderSize; ImGui::SameLine(); ImGui::DragScalarN("ChildBorderSize", ImGuiDataType_Float, &style.ChildBorderSize, sizeof(style.ChildBorderSize) / sizeof(float)); ImGui::PopID();
  ImGui::PushID("PopupRounding"); if (ImGui::Button("Reset")) style.PopupRounding = defaultStyle.PopupRounding; ImGui::SameLine(); ImGui::DragScalarN("PopupRounding", ImGuiDataType_Float, &style.PopupRounding, sizeof(style.PopupRounding) / sizeof(float)); ImGui::PopID();
  ImGui::PushID("PopupBorderSize"); if (ImGui::Button("Reset")) style.PopupBorderSize = defaultStyle.PopupBorderSize; ImGui::SameLine(); ImGui::DragScalarN("PopupBorderSize", ImGuiDataType_Float, &style.PopupBorderSize, sizeof(style.PopupBorderSize) / sizeof(float)); ImGui::PopID();
  ImGui::PushID("FramePadding"); if (ImGui::Button("Reset")) style.FramePadding = defaultStyle.FramePadding; ImGui::SameLine(); ImGui::DragScalarN("FramePadding", ImGuiDataType_Float, &style.FramePadding, sizeof(style.FramePadding) / sizeof(float)); ImGui::PopID();
  ImGui::PushID("FrameRounding"); if (ImGui::Button("Reset")) style.FrameRounding = defaultStyle.FrameRounding; ImGui::SameLine(); ImGui::DragScalarN("FrameRounding", ImGuiDataType_Float, &style.FrameRounding, sizeof(style.FrameRounding) / sizeof(float)); ImGui::PopID();
  ImGui::PushID("FrameBorderSize"); if (ImGui::Button("Reset")) style.FrameBorderSize = defaultStyle.FrameBorderSize; ImGui::SameLine(); ImGui::DragScalarN("FrameBorderSize", ImGuiDataType_Float, &style.FrameBorderSize, sizeof(style.FrameBorderSize) / sizeof(float)); ImGui::PopID();
  ImGui::PushID("ItemSpacing"); if (ImGui::Button("Reset")) style.ItemSpacing = defaultStyle.ItemSpacing; ImGui::SameLine(); ImGui::DragScalarN("ItemSpacing", ImGuiDataType_Float, &style.ItemSpacing, sizeof(style.ItemSpacing) / sizeof(float)); ImGui::PopID();
  ImGui::PushID("ItemInnerSpacing"); if (ImGui::Button("Reset")) style.ItemInnerSpacing = defaultStyle.ItemInnerSpacing; ImGui::SameLine(); ImGui::DragScalarN("ItemInnerSpacing", ImGuiDataType_Float, &style.ItemInnerSpacing, sizeof(style.ItemInnerSpacing) / sizeof(float)); ImGui::PopID();
  ImGui::PushID("CellPadding"); if (ImGui::Button("Reset")) style.CellPadding = defaultStyle.CellPadding; ImGui::SameLine(); ImGui::DragScalarN("CellPadding", ImGuiDataType_Float, &style.CellPadding, sizeof(style.CellPadding) / sizeof(float)); ImGui::PopID();
  ImGui::PushID("TouchExtraPadding"); if (ImGui::Button("Reset")) style.TouchExtraPadding = defaultStyle.TouchExtraPadding; ImGui::SameLine(); ImGui::DragScalarN("TouchExtraPadding", ImGuiDataType_Float, &style.TouchExtraPadding, sizeof(style.TouchExtraPadding) / sizeof(float)); ImGui::PopID();
  ImGui::PushID("IndentSpacing"); if (ImGui::Button("Reset")) style.IndentSpacing = defaultStyle.IndentSpacing; ImGui::SameLine(); ImGui::DragScalarN("IndentSpacing", ImGuiDataType_Float, &style.IndentSpacing, sizeof(style.IndentSpacing) / sizeof(float)); ImGui::PopID();
  ImGui::PushID("ColumnsMinSpacing"); if (ImGui::Button("Reset")) style.ColumnsMinSpacing = defaultStyle.ColumnsMinSpacing; ImGui::SameLine(); ImGui::DragScalarN("ColumnsMinSpacing", ImGuiDataType_Float, &style.ColumnsMinSpacing, sizeof(style.ColumnsMinSpacing) / sizeof(float)); ImGui::PopID();
  ImGui::PushID("ScrollbarSize"); if (ImGui::Button("Reset")) style.ScrollbarSize = defaultStyle.ScrollbarSize; ImGui::SameLine(); ImGui::DragScalarN("ScrollbarSize", ImGuiDataType_Float, &style.ScrollbarSize, sizeof(style.ScrollbarSize) / sizeof(float)); ImGui::PopID();
  ImGui::PushID("ScrollbarRounding"); if (ImGui::Button("Reset")) style.ScrollbarRounding = defaultStyle.ScrollbarRounding; ImGui::SameLine(); ImGui::DragScalarN("ScrollbarRounding", ImGuiDataType_Float, &style.ScrollbarRounding, sizeof(style.ScrollbarRounding) / sizeof(float)); ImGui::PopID();
  ImGui::PushID("GrabMinSize"); if (ImGui::Button("Reset")) style.GrabMinSize = defaultStyle.GrabMinSize; ImGui::SameLine(); ImGui::DragScalarN("GrabMinSize", ImGuiDataType_Float, &style.GrabMinSize, sizeof(style.GrabMinSize) / sizeof(float)); ImGui::PopID();
  ImGui::PushID("GrabRounding"); if (ImGui::Button("Reset")) style.GrabRounding = defaultStyle.GrabRounding; ImGui::SameLine(); ImGui::DragScalarN("GrabRounding", ImGuiDataType_Float, &style.GrabRounding, sizeof(style.GrabRounding) / sizeof(float)); ImGui::PopID();
  ImGui::PushID("LogSliderDeadzone"); if (ImGui::Button("Reset")) style.LogSliderDeadzone = defaultStyle.LogSliderDeadzone; ImGui::SameLine(); ImGui::DragScalarN("LogSliderDeadzone", ImGuiDataType_Float, &style.LogSliderDeadzone, sizeof(style.LogSliderDeadzone) / sizeof(float)); ImGui::PopID();
  ImGui::PushID("ImageBorderSize"); if (ImGui::Button("Reset")) style.ImageBorderSize = defaultStyle.ImageBorderSize; ImGui::SameLine(); ImGui::DragScalarN("ImageBorderSize", ImGuiDataType_Float, &style.ImageBorderSize, sizeof(style.ImageBorderSize) / sizeof(float)); ImGui::PopID();
  ImGui::PushID("TabRounding"); if (ImGui::Button("Reset")) style.TabRounding = defaultStyle.TabRounding; ImGui::SameLine(); ImGui::DragScalarN("TabRounding", ImGuiDataType_Float, &style.TabRounding, sizeof(style.TabRounding) / sizeof(float)); ImGui::PopID();
  ImGui::PushID("TabBorderSize"); if (ImGui::Button("Reset")) style.TabBorderSize = defaultStyle.TabBorderSize; ImGui::SameLine(); ImGui::DragScalarN("TabBorderSize", ImGuiDataType_Float, &style.TabBorderSize, sizeof(style.TabBorderSize) / sizeof(float)); ImGui::PopID();
  ImGui::PushID("TabCloseButtonMinWidthSelected"); if (ImGui::Button("Reset")) style.TabCloseButtonMinWidthSelected = defaultStyle.TabCloseButtonMinWidthSelected; ImGui::SameLine(); ImGui::DragScalarN("TabCloseButtonMinWidthSelected", ImGuiDataType_Float, &style.TabCloseButtonMinWidthSelected, sizeof(style.TabCloseButtonMinWidthSelected) / sizeof(float)); ImGui::PopID();
  ImGui::PushID("TabCloseButtonMinWidthUnselected"); if (ImGui::Button("Reset")) style.TabCloseButtonMinWidthUnselected = defaultStyle.TabCloseButtonMinWidthUnselected; ImGui::SameLine(); ImGui::DragScalarN("TabCloseButtonMinWidthUnselected", ImGuiDataType_Float, &style.TabCloseButtonMinWidthUnselected, sizeof(style.TabCloseButtonMinWidthUnselected) / sizeof(float)); ImGui::PopID();
  ImGui::PushID("TabBarBorderSize"); if (ImGui::Button("Reset")) style.TabBarBorderSize = defaultStyle.TabBarBorderSize; ImGui::SameLine(); ImGui::DragScalarN("TabBarBorderSize", ImGuiDataType_Float, &style.TabBarBorderSize, sizeof(style.TabBarBorderSize) / sizeof(float)); ImGui::PopID();
  ImGui::PushID("TabBarOverlineSize"); if (ImGui::Button("Reset")) style.TabBarOverlineSize = defaultStyle.TabBarOverlineSize; ImGui::SameLine(); ImGui::DragScalarN("TabBarOverlineSize", ImGuiDataType_Float, &style.TabBarOverlineSize, sizeof(style.TabBarOverlineSize) / sizeof(float)); ImGui::PopID();
  ImGui::PushID("TableAngledHeadersAngle"); if (ImGui::Button("Reset")) style.TableAngledHeadersAngle = defaultStyle.TableAngledHeadersAngle; ImGui::SameLine(); ImGui::DragScalarN("TableAngledHeadersAngle", ImGuiDataType_Float, &style.TableAngledHeadersAngle, sizeof(style.TableAngledHeadersAngle) / sizeof(float)); ImGui::PopID();
  ImGui::PushID("TableAngledHeadersTextAlign"); if (ImGui::Button("Reset")) style.TableAngledHeadersTextAlign = defaultStyle.TableAngledHeadersTextAlign; ImGui::SameLine(); ImGui::DragScalarN("TableAngledHeadersTextAlign", ImGuiDataType_Float, &style.TableAngledHeadersTextAlign, sizeof(style.TableAngledHeadersTextAlign) / sizeof(float)); ImGui::PopID();
  
  static constexpr std::array<const char*, 3> s_treeLignesTexts =
  {
    "None",
    "Full",
    "ToNodes",
  };

  static std::map<ImGuiTreeNodeFlags, size_t> assoc =
  {
    { ImGuiTreeNodeFlags_DrawLinesNone, 0},
    { ImGuiTreeNodeFlags_DrawLinesFull, 1},
    { ImGuiTreeNodeFlags_DrawLinesToNodes, 2},
  };

  int currentFlagId = assoc[style.TreeLinesFlags];

  if (ImGui::BeginCombo("TreeLinesFlags", s_treeLignesTexts[currentFlagId]))
  {
    for (size_t i = 0; i < s_treeLignesTexts.size(); ++i)
    {
      if (ImGui::Selectable(s_treeLignesTexts[i], i == currentFlagId))
      {
        style.TreeLinesFlags = ImGuiTreeNodeFlags_DrawLinesNone << i;
      }
    }

    ImGui::EndCombo();
  }

  ImGui::PushID("TreeLinesSize"); if (ImGui::Button("Reset")) style.TreeLinesSize = defaultStyle.TreeLinesSize; ImGui::SameLine(); ImGui::DragScalarN("TreeLinesSize", ImGuiDataType_Float, &style.TreeLinesSize, sizeof(style.TreeLinesSize) / sizeof(float)); ImGui::PopID();
  ImGui::PushID("TreeLinesRounding"); if (ImGui::Button("Reset")) style.TreeLinesRounding = defaultStyle.TreeLinesRounding; ImGui::SameLine(); ImGui::DragScalarN("TreeLinesRounding", ImGuiDataType_Float, &style.TreeLinesRounding, sizeof(style.TreeLinesRounding) / sizeof(float)); ImGui::PopID();
  // ImGui::DragScalarN("ColorButtonPosition", ImGuiDataType_Float, &style.ColorButtonPosition, sizeof(style.ColorButtonPosition) / sizeof(float));
  ImGui::PushID("ButtonTextAlign"); if (ImGui::Button("Reset")) style.ButtonTextAlign = defaultStyle.ButtonTextAlign; ImGui::SameLine(); ImGui::DragScalarN("ButtonTextAlign", ImGuiDataType_Float, &style.ButtonTextAlign, sizeof(style.ButtonTextAlign) / sizeof(float)); ImGui::PopID();
  ImGui::PushID("SelectableTextAlign"); if (ImGui::Button("Reset")) style.SelectableTextAlign = defaultStyle.SelectableTextAlign; ImGui::SameLine(); ImGui::DragScalarN("SelectableTextAlign", ImGuiDataType_Float, &style.SelectableTextAlign, sizeof(style.SelectableTextAlign) / sizeof(float)); ImGui::PopID();
  ImGui::PushID("SeparatorTextBorderSize"); if (ImGui::Button("Reset")) style.SeparatorTextBorderSize = defaultStyle.SeparatorTextBorderSize; ImGui::SameLine(); ImGui::DragScalarN("SeparatorTextBorderSize", ImGuiDataType_Float, &style.SeparatorTextBorderSize, sizeof(style.SeparatorTextBorderSize) / sizeof(float)); ImGui::PopID();
  ImGui::PushID("SeparatorTextAlign"); if (ImGui::Button("Reset")) style.SeparatorTextAlign = defaultStyle.SeparatorTextAlign; ImGui::SameLine(); ImGui::DragScalarN("SeparatorTextAlign", ImGuiDataType_Float, &style.SeparatorTextAlign, sizeof(style.SeparatorTextAlign) / sizeof(float)); ImGui::PopID();
  ImGui::PushID("SeparatorTextPadding"); if (ImGui::Button("Reset")) style.SeparatorTextPadding = defaultStyle.SeparatorTextPadding; ImGui::SameLine(); ImGui::DragScalarN("SeparatorTextPadding", ImGuiDataType_Float, &style.SeparatorTextPadding, sizeof(style.SeparatorTextPadding) / sizeof(float)); ImGui::PopID();
  ImGui::PushID("DisplayWindowPadding"); if (ImGui::Button("Reset")) style.DisplayWindowPadding = defaultStyle.DisplayWindowPadding; ImGui::SameLine(); ImGui::DragScalarN("DisplayWindowPadding", ImGuiDataType_Float, &style.DisplayWindowPadding, sizeof(style.DisplayWindowPadding) / sizeof(float)); ImGui::PopID();
  ImGui::PushID("DisplaySafeAreaPadding"); if (ImGui::Button("Reset")) style.DisplaySafeAreaPadding = defaultStyle.DisplaySafeAreaPadding; ImGui::SameLine(); ImGui::DragScalarN("DisplaySafeAreaPadding", ImGuiDataType_Float, &style.DisplaySafeAreaPadding, sizeof(style.DisplaySafeAreaPadding) / sizeof(float)); ImGui::PopID();
  ImGui::PushID("DockingSeparatorSize"); if (ImGui::Button("Reset")) style.DockingSeparatorSize = defaultStyle.DockingSeparatorSize; ImGui::SameLine(); ImGui::DragScalarN("DockingSeparatorSize", ImGuiDataType_Float, &style.DockingSeparatorSize, sizeof(style.DockingSeparatorSize) / sizeof(float)); ImGui::PopID();
  ImGui::PushID("MouseCursorScale"); if (ImGui::Button("Reset")) style.MouseCursorScale = defaultStyle.MouseCursorScale; ImGui::SameLine(); ImGui::DragScalarN("MouseCursorScale", ImGuiDataType_Float, &style.MouseCursorScale, sizeof(style.MouseCursorScale) / sizeof(float)); ImGui::PopID();
  ImGui::PushID("AntiAliasedLines"); if (ImGui::Button("Reset")) style.AntiAliasedLines = defaultStyle.AntiAliasedLines; ImGui::SameLine(); ImGui::Checkbox("AntiAliasedLines", &style.AntiAliasedLines); ImGui::PopID();
  ImGui::PushID("AntiAliasedLinesUseTex"); if (ImGui::Button("Reset")) style.AntiAliasedLinesUseTex = defaultStyle.AntiAliasedLinesUseTex; ImGui::SameLine(); ImGui::Checkbox("AntiAliasedLinesUseTex", &style.AntiAliasedLinesUseTex); ImGui::PopID();
  ImGui::PushID("AntiAliasedFill"); if (ImGui::Button("Reset")) style.AntiAliasedFill = defaultStyle.AntiAliasedFill; ImGui::SameLine(); ImGui::Checkbox("AntiAliasedFill", &style.AntiAliasedFill); ImGui::PopID();
  ImGui::PushID("CurveTessellationTol"); if (ImGui::Button("Reset")) style.CurveTessellationTol = defaultStyle.CurveTessellationTol; ImGui::SameLine(); ImGui::DragScalarN("CurveTessellationTol", ImGuiDataType_Float, &style.CurveTessellationTol, sizeof(style.CurveTessellationTol) / sizeof(float)); ImGui::PopID();
  ImGui::PushID("CircleTessellationMaxError"); if (ImGui::Button("Reset")) style.CircleTessellationMaxError = defaultStyle.CircleTessellationMaxError; ImGui::SameLine(); ImGui::DragScalarN("CircleTessellationMaxError", ImGuiDataType_Float, &style.CircleTessellationMaxError, sizeof(style.CircleTessellationMaxError) / sizeof(float)); ImGui::PopID();
}
