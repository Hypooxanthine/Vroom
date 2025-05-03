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

    if (ImGui::BeginCombo("Antialiasing", std::to_string(*m_currentMsaaValue).c_str()))
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

  }
  ImGui::End();

  if (settingsChanged)
  {
    Renderer::Get().setRenderSettings(settings);
  }

  return ret;
}
