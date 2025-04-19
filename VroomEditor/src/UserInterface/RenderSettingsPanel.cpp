#include "VroomEditor/UserInterface/RenderSettingsPanel.h"

#include <numeric>
#include <imgui.h>

#include <Vroom/Core/Application.h>
#include <Vroom/Core/GameLayer.h>
#include <Vroom/Render/Renderer.h>

using namespace vrm;

RenderSettingsPanel::RenderSettingsPanel()
{
  const auto& features = Renderer::Get().getGPUFeatures();
  

  for (uint8_t i = 1; i <= features.maxMSAA; i = i * 2)
  {
    m_msaaPossibleValues.emplace_back(i);
  }
}

RenderSettingsPanel::~RenderSettingsPanel()
{
}

bool RenderSettingsPanel::onImgui()
{
  bool ret = false;

  if (ImGui::Begin("Render settings"))
  {
    static std::array<std::string, 5> possibleValues = { "1", "2", "4", "8", "16" };
    static uint8_t currentVal = 0;

    if (ImGui::BeginCombo("Antialiasing", std::to_string(m_msaaPossibleValues.at(currentVal)).c_str()))
    {
      for (uint8_t i = 0, imax = static_cast<uint8_t>(m_msaaPossibleValues.size()); i < imax; ++i)
      {
        const auto& valStr = std::to_string(m_msaaPossibleValues.at(i));
        bool selected = currentVal == i;

        if (ImGui::Selectable(valStr.c_str(), &selected))
        {
          currentVal = i;
          uint8_t aa = static_cast<uint8_t>(std::pow(2, i));

          auto settings = Renderer::Get().getRenderSettings();
          settings.antiAliasingLevel = aa;
          Renderer::Get().setRenderSettings(settings);
        }
      }

      ImGui::EndCombo();
    }
  }
  ImGui::End();

  return ret;
}
