#pragma once

#include "VroomEditor/UserInterface/ImGuiElement.h"

#include "imgui.h"

namespace vrm
{

  class EditorPreferences : public ImGuiElement
  {
  public:
    EditorPreferences();
    ~EditorPreferences();

  protected:
    void onImgui() override;

    void _editImguiStyle();

  private:

    ImGuiStyle m_imguiStyle;

  };

} // namespace vrm
