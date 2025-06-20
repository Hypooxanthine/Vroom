#pragma once

#include "VroomEditor/UserInterface/ImGuiElement.h"

namespace vrm
{

class EditorPreferences : public ImGuiElement
{
public:
  EditorPreferences();
  ~EditorPreferences();

protected:
  bool onImgui() override;

};

} // namespace vrm
