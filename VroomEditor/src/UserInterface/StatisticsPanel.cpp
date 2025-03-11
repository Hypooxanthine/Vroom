#include "VroomEditor/UserInterface/StatisticsPanel.h"

#include <imgui.h>

namespace vrm
{

StatisticsPanel::StatisticsPanel()
    : frameTime(0.f)
{
}

StatisticsPanel::~StatisticsPanel()
{
}

bool StatisticsPanel::onImgui()
{
    bool ret = false;

    ImGui::Begin("Statistics");

    ImGui::Text("Frame time: %.5f s", frameTime);
    ImGui::Text("Frame rate: %.0f FPS", 1.f / frameTime);

    ImGui::End();

    return ret;
}

} // namespace vrm