#include "VroomEditor/UserInterface/RenderSettingsPanel.h"

#include <numeric>
#include <imgui.h>

#include <Vroom/Core/Application.h>
#include <Vroom/Core/GameLayer.h>
#include <Vroom/Render/Renderer.h>

using namespace vrm;

RenderSettingsPanel::RenderSettingsPanel()
{
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
