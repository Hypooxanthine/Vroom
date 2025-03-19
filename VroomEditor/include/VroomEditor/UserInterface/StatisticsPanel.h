#pragma once

#include "VroomEditor/UserInterface/ImGuiElement.h"

namespace vrm
{

class StatisticsPanel : public ImGuiElement
{
public:
    StatisticsPanel();
    ~StatisticsPanel();

protected:
    bool onImgui() override;

public: // Public ImGui related variables

};

} // namespace vrm