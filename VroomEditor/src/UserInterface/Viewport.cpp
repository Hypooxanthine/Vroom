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
        ImTextureID textureID = (void*)(intptr_t)frameBuffer->getTexture().getRendererID();
        ImVec2 imageSize = ImVec2(
            static_cast<float>(frameBuffer->getSpecification().width),
            static_cast<float>(frameBuffer->getSpecification().height)
        );
        ImGui::Image(textureID, imageSize, ImVec2(0, 1), ImVec2(1, 0));
    }

    ImGui::End();
}

} // namespace vrm