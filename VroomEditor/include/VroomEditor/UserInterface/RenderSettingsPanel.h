#pragma once

#include <vector>
#include <cstdint>
#include <string>

#include "VroomEditor/UserInterface/ImGuiElement.h"
#include <Vroom/Render/Camera/FirstPersonCamera.h>

namespace vrm
{

  class RenderSettingsPanel : public ImGuiElement
  {
  public:
    RenderSettingsPanel();
    ~RenderSettingsPanel();

  protected:
    bool onImgui() override;

  private:
    std::vector<uint8_t> m_msaaPossibleValues;
    std::string m_watchedTexture = "";
    int m_dummyCameraUsed = 0;

    std::array<FirstPersonCamera, 3> m_dummyCameras =
    {
      FirstPersonCamera(0.1f, 100.f, 90.f, 0.f, glm::vec3(10.f, 0.f, 0.f), glm::vec3(0.f, glm::radians(-90.f), 0.f)),
      FirstPersonCamera(0.1f, 100.f, 90.f, 0.f, glm::vec3(0.f, 0.f, 10.f), glm::vec3(0.f, glm::radians(0.f), 0.f)),
      FirstPersonCamera(0.1f, 100.f, 90.f, 0.f, glm::vec3(-10.f, 0.f, 0.f), glm::vec3(0.f, glm::radians(90.f), 0.f)),
    };

  };

} // namespace vrm
