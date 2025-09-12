#include "VroomEditor/UserInterface/ProfilingPanel.h"

#include "imgui.h"

#include "implot.h"

using namespace vrm;
using namespace vrm::editor;

ProfilingPanel::ProfilingPanel() {}

ProfilingPanel::~ProfilingPanel() {}

void ProfilingPanel::onImgui()
{
  if (ImGui::Begin("Profiling", m_open))
  {
    //
  }
  ImGui::End();
}
