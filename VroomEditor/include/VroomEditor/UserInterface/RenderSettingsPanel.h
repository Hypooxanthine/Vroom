#pragma once

#include <vector>
#include <cstdint>

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

  protected:
    std::vector<uint8_t> m_msaaPossibleValues;
    decltype(m_msaaPossibleValues)::iterator m_currentMsaaValue;
  };

} // namespace vrm
