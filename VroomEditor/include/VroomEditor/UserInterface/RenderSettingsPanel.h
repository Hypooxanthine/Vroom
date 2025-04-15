#pragma once

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
  };

} // namespace vrm
