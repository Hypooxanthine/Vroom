#pragma once

#include "VroomEditor/UserInterface/ImGuiElement.h"

namespace vrm
{

class MainMenuBar : public ImGuiElement
{
public:
    MainMenuBar();
    ~MainMenuBar();

protected:
    bool onImgui() override;

};

} // namespace vrm
