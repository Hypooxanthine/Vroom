#include "VroomEditor/UserInterface/Viewport.h"

#include <Vroom/Render/Abstraction/FrameBuffer.h>

#include "imgui.h"

namespace vrm
{

Viewport::Viewport()
    : frameBuffer(nullptr), m_LastViewportSize(ImVec2(0.f, 0.f)), m_DidSizeChangeLastFrame(false)
{
}

Viewport::~Viewport()
{
}

void Viewport::onImgui()
{
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));

    ImGui::Begin("Viewport");

    auto size = ImGui::GetContentRegionAvail();

    if (size.x != m_LastViewportSize.x || size.y != m_LastViewportSize.y)
    {
        m_DidSizeChangeLastFrame = true;
        m_LastViewportSize = size;
    }
    else
    {
        m_DidSizeChangeLastFrame = false;
    }

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

    ImGui::PopStyleVar();
}

} // namespace vrm