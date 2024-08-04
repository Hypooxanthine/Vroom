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
    void onImgui() override;

public: // Public ImGui related variables
    float frameTime;

};

} // namespace vrm