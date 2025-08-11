#include "VroomEditor/UserInterface/RenderSettingsPanel.h"

#include <imgui.h>

#include <Vroom/Core/Application.h>
#include <Vroom/Core/GameLayer.h>
#include <Vroom/Scene/Scene.h>
#include <Vroom/Render/Renderer.h>

using namespace vrm;

RenderSettingsPanel::RenderSettingsPanel()
{
  const auto& features = Application::Get().getMainSceneRenderer().getGPUFeatures();

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
  auto& renderer = Application::Get().getMainSceneRenderer();
  auto settings = renderer.getRenderSettings();
  auto dynSettings = renderer.getDynamicRenderSettings();
  bool settingsChanged = false;
  bool dynSettingsChanged = false;

  if (ImGui::Begin("Render settings", m_open))
  {

    int framerateLimit = static_cast<int>(Application::Get().getFrameRateLimit());
    ImGui::TextWrapped("Framerate limit:");
    if (ImGui::SliderInt("##Framerate limit", &framerateLimit, 0, 360, "%d", ImGuiSliderFlags_AlwaysClamp))
    {
      settings.frameRateLimit = static_cast<uint16_t>(framerateLimit);
      settingsChanged = true;
    }

    ImGui::TextWrapped("Antialiasing:");
    if (ImGui::BeginCombo("##Antialiasing", std::to_string(settings.antiAliasingLevel).c_str()))
    {
      for (auto it = m_msaaPossibleValues.begin(); it != m_msaaPossibleValues.end(); ++it)
      {
        auto itemValue = *it;
        const auto& valStr = std::to_string(itemValue);
        bool selected = (settings.antiAliasingLevel == itemValue);

        if (ImGui::Selectable(valStr.c_str(), &selected))
        {
          settings.antiAliasingLevel = itemValue;
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
    if (ImGui::SliderInt("##Soft shadows kernel radius", &softShadowKernelRadius, 0, 10, "%d", ImGuiSliderFlags_AlwaysClamp))
    {
      dynSettings.shadows.softShadowKernelRadius = static_cast<uint8_t>(softShadowKernelRadius);
      dynSettingsChanged = true;
    }

    if (ImGui::SliderFloat("exposure", &dynSettings.hdr.exposure, 0.f, 10.f, "%.2f", ImGuiSliderFlags_AlwaysClamp))
    {
      dynSettingsChanged = true;
    }

    if (ImGui::Checkbox("Bloom", &settings.bloom.activated))
    {
      settingsChanged = true;
    }

    if (settings.bloom.activated)
    {
      if (ImGui::SliderFloat("Bloom intensity", &dynSettings.bloom.intensity, 0.f, 10.f))
      {
        dynSettingsChanged = true;
      }

      if (ImGui::SliderFloat("Bloom threshold", &dynSettings.bloom.threshold, 0.f, 10.f))
      {
        dynSettingsChanged = true;
      }

      int blurPasses = static_cast<int>(dynSettings.bloom.blurPasses);
      int min = 1;
      int max = std::numeric_limits<decltype(dynSettings.bloom.blurPasses)>::max();

      if (ImGui::SliderInt("Bloom blur passes", &blurPasses, min, max, "%d", ImGuiSliderFlags_AlwaysClamp))
      {
        dynSettings.bloom.blurPasses = blurPasses;
        dynSettingsChanged = true;
      }
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

    const std::string& watchedTexture = renderer.getWatchedTexture();

    if (ImGui::BeginCombo("Watch texture", watchedTexture.empty() ? "Default" : watchedTexture.c_str()))
    {
      bool selected = (watchedTexture == "");
      if (ImGui::Selectable("Default", selected))
      {
        renderer.watchExposedTexture("");
      }

      for (const std::string& texName : Application::Get().getMainSceneRenderer().getExposedTextureNames())
      {
        selected = (watchedTexture == texName);
        if (ImGui::Selectable(texName.c_str(), selected))
        {
          renderer.watchExposedTexture(texName);
        }
      }

      ImGui::EndCombo();
    }

    if (ImGui::SliderInt("Dummy cameras", &m_dummyCameraUsed, 0, 3, "%d", ImGuiSliderFlags_AlwaysClamp))
    {
      Scene& scene = Application::Get().getGameLayer().getScene();
      scene.setSplitScreenGridSize(1, 1); // First removing all cameras

      if (m_dummyCameraUsed == 1)
      {
        // Two cameras
        scene.setSplitScreenGridSize(2, 1);
        scene.setCamera(&m_dummyCameras.at(0), 1, 0);
      }
      else if (m_dummyCameraUsed > 1)
      {
        // 3 or 4 cameras
        scene.setSplitScreenGridSize(2, 2);

        for (int i = 0; i < m_dummyCameraUsed; ++i)
        {
          scene.setCamera(&m_dummyCameras.at(i), (i + 1) / 2, (i + 1) % 2);
        }
      }
    }

    if (ImGui::Checkbox("Show light complexity", &settings.showLightComplexity))
    {
      settingsChanged = true;
    }
  }
  ImGui::End();

  if (settingsChanged)
  {
    Application::Get().getMainSceneRenderer().setRenderSettings(settings);
  }

  if (dynSettingsChanged)
  {
    Application::Get().getMainSceneRenderer().setDynamicRenderSettings(dynSettings);
  }

  return ret;
}
