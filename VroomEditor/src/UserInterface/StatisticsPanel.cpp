#include "VroomEditor/UserInterface/StatisticsPanel.h"

#include <numeric>
#include <imgui.h>

#include <Vroom/Core/Application.h>
#include <Vroom/Core/GameLayer.h>

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
  static float meanDt = dt;
  static size_t framesCounter = 0;
  static float meanFps = 1.f / dt;
  
  static float timeAccumulator = 0.f;
  constexpr float sampleTime = 2.f;

  timeAccumulator += dt;
  ++framesCounter;

  if (timeAccumulator > sampleTime)
  {
    meanFps = static_cast<float>(framesCounter) / sampleTime;
    meanDt = 1.f / meanFps;
    timeAccumulator -= sampleTime;
    framesCounter = 0;
  }

  if (ImGui::Begin("Statistics"))
  {
    ImGui::Text("Frame time: %.5f s", meanDt);
    ImGui::Text("Frame rate: %.0f FPS", meanFps);

    static std::array<std::string, 5> possibleValues = { "1", "2", "4", "8", "16" };
    static size_t currentVal = 0;

    if (ImGui::BeginCombo("Antialiasing", possibleValues.at(currentVal).c_str()))
    {
      for (size_t i = 0; i < possibleValues.size(); ++i)
      {
        const auto& val = possibleValues.at(i);
        bool selected = currentVal == i;

        if (ImGui::Selectable(val.c_str(), &selected))
        {
          currentVal = i;
          uint8_t aa = static_cast<uint8_t>(std::pow(2, i));
          Application::Get().getGameLayer().setAntialiasingLevel(aa);
        }
      }

      ImGui::EndCombo();
    }
  }
  ImGui::End();

  return ret;
}
