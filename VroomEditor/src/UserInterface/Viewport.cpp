#include "VroomEditor/UserInterface/Viewport.h"

#include <Vroom/Render/Abstraction/FrameBuffer.h>

#include "imgui.h"

namespace vrm
{

Viewport::Viewport()
    : frameBuffer(nullptr)
{
}

Viewport::~Viewport()
{
}

void Viewport::onImgui()
{
    ImGui::Begin("Viewport");

    if (frameBuffer)
    {
        ImGui::Image((void*)frameBuffer->getTexture().getRendererID(), ImVec2(1280, 720), ImVec2(0, 1), ImVec2(1, 0));
    }

    ImGui::End();
}

} // namespace vrm