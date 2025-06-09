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
  m_currentMsaaValue = m_msaaPossibleValues.begin();
}

RenderSettingsPanel::~RenderSettingsPanel()
{
}

bool RenderSettingsPanel::onImgui()
{
  bool ret = false;
  auto settings = Renderer::Get().getRenderSettings();
  bool settingsChanged = false;

  if (ImGui::Begin("Render settings"))
  {

    int framerateLimit = static_cast<int>(Application::Get().getFrameRateLimit());
    ImGui::TextWrapped("Framerate limit:");
    if (ImGui::SliderInt("##Framerate limit", &framerateLimit, 0, 360, "%d", ImGuiSliderFlags_ClampOnInput))
    {
      settings.frameRateLimit = static_cast<uint16_t>(framerateLimit);
      settingsChanged = true;
    }

    ImGui::TextWrapped("Antialiasing:");
    if (ImGui::BeginCombo("##Antialiasing", std::to_string(*m_currentMsaaValue).c_str()))
    {
      for (auto it = m_msaaPossibleValues.begin(); it != m_msaaPossibleValues.end(); ++it)
      {
        const auto& valStr = std::to_string(*it);
        bool selected = (m_currentMsaaValue == it);

        if (ImGui::Selectable(valStr.c_str(), &selected))
        {
          m_currentMsaaValue = it;

          settings.antiAliasingLevel = *it;
          settingsChanged = true;
        }
      }

      ImGui::EndCombo();
    }

    if (ImGui::Checkbox("Enable shadows", &settings.shadowsEnable))
    {
      settingsChanged = true;
    }

    int softShadowKernelRadius = static_cast<int>(settings.softShadowKernelRadius);

    ImGui::TextWrapped("Soft shadows kernel radius:");
    if (ImGui::SliderInt("##Soft shadows kernel radius", &softShadowKernelRadius, 0, 10, "%d", ImGuiSliderFlags_ClampOnInput))
    {
      settings.softShadowKernelRadius = static_cast<uint8_t>(softShadowKernelRadius);
      settingsChanged = true;
    }

  }
  ImGui::End();

  if (settingsChanged)
  {
    Renderer::Get().setRenderSettings(settings);
  }

  return ret;
}
