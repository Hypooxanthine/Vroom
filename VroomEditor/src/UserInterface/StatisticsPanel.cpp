#include "VroomEditor/UserInterface/StatisticsPanel.h"

#include <imgui.h>

#include <Vroom/Core/Application.h>

using namespace vrm;

StatisticsPanel::StatisticsPanel()
{
}

StatisticsPanel::~StatisticsPanel()
{
}

bool StatisticsPanel::onImgui()
{
  bool ret = false;
  float dt = Application::Get().getDeltaTime();
  float fps = 1.f / dt;

  if (ImGui::Begin("Statistics"))
  {
    ImGui::Text("Frame time: %.5f s", dt);
    ImGui::Text("Frame rate: %.0f FPS", fps);
  }
  ImGui::End();

  return ret;
}
