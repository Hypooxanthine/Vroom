#include "VroomEditor/UserInterface/StatisticsPanel.h"

#include <numeric>
#include <imgui.h>

#include <Vroom/Core/Application.h>
#include <Vroom/Core/DeltaTime.h>
#include <Vroom/Core/GameLayer.h>
#include <Vroom/Render/Renderer.h>

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

  if (ImGui::Begin("Statistics", m_open))
  {
    const DeltaTime& dt = Application::Get().getDeltaTime();
    static float meanDt = dt.seconds();
    static size_t framesCounter = 0;
    static float meanFps = 1.f / dt.seconds();
    
    static float timeAccumulator = 0.f;
    constexpr float sampleTime = 2.f;

    timeAccumulator += dt.seconds();
    ++framesCounter;

    if (timeAccumulator > sampleTime)
    {
      meanFps = static_cast<float>(framesCounter) / sampleTime;
      meanDt = 1.f / meanFps;
      timeAccumulator -= sampleTime;
      framesCounter = 0;
    }
    
    ImGui::Text("Frame time: %.5f s", meanDt);
    ImGui::Text("Frame rate: %.0f FPS", meanFps);
  }
  ImGui::End();

  return ret;
}
