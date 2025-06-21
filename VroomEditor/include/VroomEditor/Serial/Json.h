#pragma once

#include <Vroom/Asset/Parsing/Json.h>

#include "imgui.h"

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(ImVec2, x, y)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(ImVec4, x, y)

namespace nlohmann {

  template <>
  struct adl_serializer<ImGuiStyle> {
    static void to_json(json& j, const ImGuiStyle& s) {
      j = json{
        {"FontSizeBase", s.FontSizeBase},
        {"FontScaleMain", s.FontScaleMain},
        {"FontScaleDpi", s.FontScaleDpi},
        {"Alpha", s.Alpha},
        {"DisabledAlpha", s.DisabledAlpha},
        {"WindowPadding", s.WindowPadding},
        {"WindowRounding", s.WindowRounding},
        {"WindowBorderSize", s.WindowBorderSize},
        {"WindowBorderHoverPadding", s.WindowBorderHoverPadding},
        {"WindowMinSize", s.WindowMinSize},
        {"WindowTitleAlign", s.WindowTitleAlign},
        {"WindowMenuButtonPosition", s.WindowMenuButtonPosition},
        {"ChildRounding", s.ChildRounding},
        {"ChildBorderSize", s.ChildBorderSize},
        {"PopupRounding", s.PopupRounding},
        {"PopupBorderSize", s.PopupBorderSize},
        {"FramePadding", s.FramePadding},
        {"FrameRounding", s.FrameRounding},
        {"FrameBorderSize", s.FrameBorderSize},
        {"ItemSpacing", s.ItemSpacing},
        {"ItemInnerSpacing", s.ItemInnerSpacing},
        {"CellPadding", s.CellPadding},
        {"TouchExtraPadding", s.TouchExtraPadding},
        {"IndentSpacing", s.IndentSpacing},
        {"ColumnsMinSpacing", s.ColumnsMinSpacing},
        {"ScrollbarSize", s.ScrollbarSize},
        {"ScrollbarRounding", s.ScrollbarRounding},
        {"GrabMinSize", s.GrabMinSize},
        {"GrabRounding", s.GrabRounding},
        {"LogSliderDeadzone", s.LogSliderDeadzone},
        {"ImageBorderSize", s.ImageBorderSize},
        {"TabRounding", s.TabRounding},
        {"TabBorderSize", s.TabBorderSize},
        {"TabCloseButtonMinWidthSelected", s.TabCloseButtonMinWidthSelected},
        {"TabCloseButtonMinWidthUnselected", s.TabCloseButtonMinWidthUnselected},
        {"TabBarBorderSize", s.TabBarBorderSize},
        {"TabBarOverlineSize", s.TabBarOverlineSize},
        {"TableAngledHeadersAngle", s.TableAngledHeadersAngle},
        {"TableAngledHeadersTextAlign", s.TableAngledHeadersTextAlign},
        {"TreeLinesFlags", s.TreeLinesFlags},
        {"TreeLinesSize", s.TreeLinesSize},
        {"TreeLinesRounding", s.TreeLinesRounding},
        {"ColorButtonPosition", s.ColorButtonPosition},
        {"ButtonTextAlign", s.ButtonTextAlign},
        {"SelectableTextAlign", s.SelectableTextAlign},
        {"SeparatorTextBorderSize", s.SeparatorTextBorderSize},
        {"SeparatorTextAlign", s.SeparatorTextAlign},
        {"SeparatorTextPadding", s.SeparatorTextPadding},
        {"DisplayWindowPadding", s.DisplayWindowPadding},
        {"DisplaySafeAreaPadding", s.DisplaySafeAreaPadding},
        {"DockingSeparatorSize", s.DockingSeparatorSize},
        {"MouseCursorScale", s.MouseCursorScale},
        {"AntiAliasedLines", s.AntiAliasedLines},
        {"AntiAliasedLinesUseTex", s.AntiAliasedLinesUseTex},
        {"AntiAliasedFill", s.AntiAliasedFill},
        {"CurveTessellationTol", s.CurveTessellationTol},
        {"CircleTessellationMaxError", s.CircleTessellationMaxError},
        {"HoverStationaryDelay", s.HoverStationaryDelay},
        {"HoverDelayShort", s.HoverDelayShort},
        {"HoverDelayNormal", s.HoverDelayNormal},
        {"HoverFlagsForTooltipMouse", s.HoverFlagsForTooltipMouse},
        {"HoverFlagsForTooltipNav", s.HoverFlagsForTooltipNav}
      };

      json colors_json = json::array();
      for (int i = 0; i < ImGuiCol_COUNT; ++i)
        colors_json.push_back({ s.Colors[i].x, s.Colors[i].y, s.Colors[i].z, s.Colors[i].w });

      j["Colors"] = colors_json;
    }

    static void from_json(const json& j, ImGuiStyle& s) {
      j.at("FontSizeBase").get_to(s.FontSizeBase);
      j.at("FontScaleMain").get_to(s.FontScaleMain);
      j.at("FontScaleDpi").get_to(s.FontScaleDpi);
      j.at("Alpha").get_to(s.Alpha);
      j.at("DisabledAlpha").get_to(s.DisabledAlpha);
      j.at("WindowPadding").get_to(s.WindowPadding);
      j.at("WindowRounding").get_to(s.WindowRounding);
      j.at("WindowBorderSize").get_to(s.WindowBorderSize);
      j.at("WindowBorderHoverPadding").get_to(s.WindowBorderHoverPadding);
      j.at("WindowMinSize").get_to(s.WindowMinSize);
      j.at("WindowTitleAlign").get_to(s.WindowTitleAlign);
      j.at("WindowMenuButtonPosition").get_to(s.WindowMenuButtonPosition);
      j.at("ChildRounding").get_to(s.ChildRounding);
      j.at("ChildBorderSize").get_to(s.ChildBorderSize);
      j.at("PopupRounding").get_to(s.PopupRounding);
      j.at("PopupBorderSize").get_to(s.PopupBorderSize);
      j.at("FramePadding").get_to(s.FramePadding);
      j.at("FrameRounding").get_to(s.FrameRounding);
      j.at("FrameBorderSize").get_to(s.FrameBorderSize);
      j.at("ItemSpacing").get_to(s.ItemSpacing);
      j.at("ItemInnerSpacing").get_to(s.ItemInnerSpacing);
      j.at("CellPadding").get_to(s.CellPadding);
      j.at("TouchExtraPadding").get_to(s.TouchExtraPadding);
      j.at("IndentSpacing").get_to(s.IndentSpacing);
      j.at("ColumnsMinSpacing").get_to(s.ColumnsMinSpacing);
      j.at("ScrollbarSize").get_to(s.ScrollbarSize);
      j.at("ScrollbarRounding").get_to(s.ScrollbarRounding);
      j.at("GrabMinSize").get_to(s.GrabMinSize);
      j.at("GrabRounding").get_to(s.GrabRounding);
      j.at("LogSliderDeadzone").get_to(s.LogSliderDeadzone);
      j.at("ImageBorderSize").get_to(s.ImageBorderSize);
      j.at("TabRounding").get_to(s.TabRounding);
      j.at("TabBorderSize").get_to(s.TabBorderSize);
      j.at("TabCloseButtonMinWidthSelected").get_to(s.TabCloseButtonMinWidthSelected);
      j.at("TabCloseButtonMinWidthUnselected").get_to(s.TabCloseButtonMinWidthUnselected);
      j.at("TabBarBorderSize").get_to(s.TabBarBorderSize);
      j.at("TabBarOverlineSize").get_to(s.TabBarOverlineSize);
      j.at("TableAngledHeadersAngle").get_to(s.TableAngledHeadersAngle);
      j.at("TableAngledHeadersTextAlign").get_to(s.TableAngledHeadersTextAlign);
      j.at("TreeLinesFlags").get_to(s.TreeLinesFlags);
      j.at("TreeLinesSize").get_to(s.TreeLinesSize);
      j.at("TreeLinesRounding").get_to(s.TreeLinesRounding);
      j.at("ColorButtonPosition").get_to(s.ColorButtonPosition);
      j.at("ButtonTextAlign").get_to(s.ButtonTextAlign);
      j.at("SelectableTextAlign").get_to(s.SelectableTextAlign);
      j.at("SeparatorTextBorderSize").get_to(s.SeparatorTextBorderSize);
      j.at("SeparatorTextAlign").get_to(s.SeparatorTextAlign);
      j.at("SeparatorTextPadding").get_to(s.SeparatorTextPadding);
      j.at("DisplayWindowPadding").get_to(s.DisplayWindowPadding);
      j.at("DisplaySafeAreaPadding").get_to(s.DisplaySafeAreaPadding);
      j.at("DockingSeparatorSize").get_to(s.DockingSeparatorSize);
      j.at("MouseCursorScale").get_to(s.MouseCursorScale);
      j.at("AntiAliasedLines").get_to(s.AntiAliasedLines);
      j.at("AntiAliasedLinesUseTex").get_to(s.AntiAliasedLinesUseTex);
      j.at("AntiAliasedFill").get_to(s.AntiAliasedFill);
      j.at("CurveTessellationTol").get_to(s.CurveTessellationTol);
      j.at("CircleTessellationMaxError").get_to(s.CircleTessellationMaxError);
      j.at("HoverStationaryDelay").get_to(s.HoverStationaryDelay);
      j.at("HoverDelayShort").get_to(s.HoverDelayShort);
      j.at("HoverDelayNormal").get_to(s.HoverDelayNormal);
      j.at("HoverFlagsForTooltipMouse").get_to(s.HoverFlagsForTooltipMouse);
      j.at("HoverFlagsForTooltipNav").get_to(s.HoverFlagsForTooltipNav);

      if (j.contains("Colors") && j["Colors"].is_array())
      {
        const auto& colors_json = j["Colors"];
        int count = std::min<int>(colors_json.size(), ImGuiCol_COUNT);
        for (int i = 0; i < count; ++i) {
          const auto& c = colors_json[i];
          if (c.is_array() && c.size() == 4) {
            s.Colors[i] = ImVec4(c[0].get<float>(), c[1].get<float>(), c[2].get<float>(), c[3].get<float>());
          }
        }
      }

    }
  };

} // namespace nlohmann
