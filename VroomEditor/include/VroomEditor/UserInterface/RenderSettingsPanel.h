#pragma once

#include <vector>
#include <cstdint>
#include <string>

#include "VroomEditor/UserInterface/ImGuiElement.h"

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

  };

} // namespace vrm
