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
  auto dynSettings = Renderer::Get().getDynamicRenderSettings();
  bool settingsChanged = false;
  bool dynSettingsChanged = false;

  if (ImGui::Begin("Render settings", m_open))
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

    int softShadowKernelRadius = static_cast<int>(dynSettings.shadows.softShadowKernelRadius);

    ImGui::TextWrapped("Soft shadows kernel radius:");
    if (ImGui::SliderInt("##Soft shadows kernel radius", &softShadowKernelRadius, 0, 10, "%d", ImGuiSliderFlags_ClampOnInput))
    {
      dynSettings.shadows.softShadowKernelRadius = static_cast<uint8_t>(softShadowKernelRadius);
      dynSettingsChanged = true;
    }

    if (ImGui::SliderFloat("exposure", &dynSettings.hdr.exposure, 0.f, 10.f, "%.2f", ImGuiSliderFlags_ClampOnInput))
    {
      dynSettingsChanged = true;
    }

    if (ImGui::Checkbox("Normal mapping", &settings.normalMapping.activated))
    {
      settingsChanged = true;
    }

    if (settings.normalMapping.activated && ImGui::Checkbox("Orthogonalize tangent space", &settings.normalMapping.reorthoTangentSpace))
    {
      settingsChanged = true;
    }

    if (ImGui::Checkbox("Clustered shading", &settings.clusteredShading))
    {
      settingsChanged = true;
    }

    if (settings.clusteredShading)
    {
      glm::ivec3 clusterCount = glm::ivec3(settings.clusterCount);
      if (ImGui::DragInt3("Cluster count", &clusterCount.x, 1.f, 1, 64, "%d"))
      {
        settings.clusterCount = glm::uvec3(clusterCount);
        settingsChanged = true;
      }
    }

    ImGui::SeparatorText("Render tools");

    if (ImGui::Checkbox("Show light complexity", &settings.showLightComplexity))
    {
      settingsChanged = true;
    }
  }
  ImGui::End();

  if (settingsChanged)
  {
    Renderer::Get().setRenderSettings(settings);
  }

  if (dynSettingsChanged)
  {
    Renderer::Get().setDynamicRenderSettings(dynSettings);
  }

  return ret;
}
