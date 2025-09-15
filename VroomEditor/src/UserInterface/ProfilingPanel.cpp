#include "VroomEditor/UserInterface/ProfilingPanel.h"
#include <vector>

#include "imgui.h"
#include "imgui_internal.h"

#include "Vroom/Core/Duration.h"
#include "Vroom/Core/PerfRecorder.h"
#include "Vroom/Core/Profiler.h"
#include "implot.h"
#include "implot_internal.h"

using namespace vrm;
using namespace vrm::editor;

ProfilingPanel::ProfilingPanel() {}

ProfilingPanel::~ProfilingPanel() {}

void ProfilingPanel::onImgui()
{
  if (ImGui::Begin("Profiling", m_open))
  {
    if (Profiler::Get().getLastFrameRoots().size() > 0)
    {
      _showRecorderTree(*Profiler::Get().getLastFrameRoots()[0]);
    }
  }
  ImGui::End();
}

void ProfilingPanel::_showRecorderTree(const PerfRecorder& recorder)
{
  if (_buildProfileEntriesCheck(recorder) == false || m_entries.empty()) return;

  static constexpr ImPlotFlags plotFlags = ImPlotFlags_NoMouseText;

  if (ImPlot::BeginPlot("Profiler Timeline", ImVec2(-1, 300), plotFlags))
  {
    const ProfileEntry& firstEntry = m_entries.front();

    ImPlot::SetupAxis(ImAxis_X1, "Time (s)",
                      ImPlotAxisFlags_AuxDefault | ImPlotAxisFlags_LockMin);
    ImPlot::SetupAxis(ImAxis_Y1, "Depth",
                      ImPlotAxisFlags_Lock | ImPlotAxisFlags_Invert
                        | ImPlotAxisFlags_NoGridLines);
    ImPlot::SetupAxisLimits(ImAxis_Y1, 0.0, (double)m_maxDepth + 1.0,
                            ImPlotCond_Always);
    ImPlot::SetupAxisLimits(ImAxis_X1, 0.0, firstEntry.endSec,
                            ImPlotCond_Always);

    auto drawList = ImPlot::GetPlotDrawList();

    for (const auto& e : m_entries)
    {
      ImGui::PushID(e.uid);
      ImVec2 p0 = ImPlot::PlotToPixels(ImPlotPoint(e.startSec, e.depth));
      ImVec2 p1 = ImPlot::PlotToPixels(ImPlotPoint(e.endSec, e.depth + 1.0));

      ImU32 color;

      if (e.depth == 0) color = IM_COL32(100, 180, 220, 255);
      else
      {
        static const constinit ImU32 colMin = IM_COL32(180, 220, 255, 255);
        static const constinit ImU32 colMax = IM_COL32(255, 170, 120, 255);

        color = _lerpColor(colMin, colMax, std::clamp(e.parentUse, 0.f, 1.f));
      }

      drawList->AddRectFilled(p0, p1, color);
      drawList->AddRect(p0, p1, IM_COL32_BLACK);

      ImVec2 textPosStart =
        ImVec2(p0.x + 3, (p0.y + p1.y) / 2 - ImGui::GetTextLineHeight() / 2);
      ImVec2 textPosEnd = p1 - (textPosStart - p0);

      ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32_BLACK);
      ImGui::RenderTextEllipsis(drawList, textPosStart, textPosEnd,
                                textPosEnd.x - textPosStart.x, e.name.c_str(),
                                nullptr, nullptr);

      ImGui::PopStyleColor();

      ImGui::SetCursorScreenPos(p0);
      ImGui::InvisibleButton(
        e.name.c_str(), ImVec2(std::max(p1.x - p0.x, 0.0001f), p1.y - p0.y));

      if (ImGui::IsItemHovered())
      {
        ImGui::BeginTooltip();
        ImGui::Text("Scope: %s", e.name.c_str());
        ImGui::Separator();
        ImGui::Text("Start:    %.6f s", e.startSec);
        ImGui::Text("End:      %.6f s", e.endSec);
        ImGui::Text("Duration: %.6f s", e.endSec - e.startSec);
        ImGui::EndTooltip();
      }

      ImGui::PopID();
    }

    ImPlot::EndPlot();
  }
}

bool ProfilingPanel::_buildProfileEntriesCheck(const PerfRecorder& recorder)
{
  if (m_timer.isStarted()
      && m_timer.getCurrentDuration<DurationSeconds>().get<float>() < 1.f)
    return true;

  m_timer.restart();

  double timeSeconds = recorder.getTimeSeconds();
  if (timeSeconds == 0.f) { return false; }

  // Most likely the same amount of entries
  size_t size = m_entries.size();
  m_entries.clear();
  m_entries.reserve(size);
  m_maxDepth = 0;

  _buildProfileEntriesRecursive(recorder, recorder.getStartTimeNanosecs(), 0);

  return true;
}

void ProfilingPanel::_buildProfileEntriesRecursive(const PerfRecorder& recorder,
                                                   uint64_t startNanosecs,
                                                   int      depth)
{
  ProfileEntry& entry = m_entries.emplace_back();

  entry.name = recorder.getName();
  entry.uid  = recorder.getId();
  entry.startSec =
    static_cast<double>(recorder.getStartTimeNanosecs() - startNanosecs)
    / DURATION_DIV_SECONDS;
  entry.endSec =
    static_cast<double>(recorder.getStartTimeNanosecs() - startNanosecs)
      / DURATION_DIV_SECONDS
    + recorder.getTimeSeconds();
  entry.depth = depth;

  if (recorder.getParent())
  {
    entry.parentUse =
      recorder.getTimeSeconds() / recorder.getParent()->getTimeSeconds();
  }

  m_maxDepth = std::max(m_maxDepth, depth);

  for (const PerfRecorder* child : recorder.getChildren())
  {
    _buildProfileEntriesRecursive(*child, startNanosecs, depth + 1);
  }
}

ImU32 ProfilingPanel::_lerpColor(ImU32 c1, ImU32 c2, float t)
{
  ImVec4 a = ImGui::ColorConvertU32ToFloat4(c1);
  ImVec4 b = ImGui::ColorConvertU32ToFloat4(c2);
  ImVec4 r = ImVec4(a.x + (b.x - a.x) * t, a.y + (b.y - a.y) * t,
                    a.z + (b.z - a.z) * t, a.w + (b.w - a.w) * t);
  return ImGui::ColorConvertFloat4ToU32(r);
};
