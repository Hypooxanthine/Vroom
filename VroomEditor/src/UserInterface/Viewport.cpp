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
    if (ImGui::Begin("Viewport"))
    {

        if (ImGui::BeginChildFrame(ImGui::GetID("ViewportTopButtons"), ImVec2(0, 30), ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse))
        {
            ImGui::SameLine((ImGui::GetWindowWidth() - 150) / 2);
        
            pressedPlay = ImGui::Button("Play");
            ImGui::SameLine();
            pressedPause = ImGui::Button("Pause");
            ImGui::SameLine();
            pressedStop = ImGui::Button("Stop");
        
            ImGui::EndChildFrame();
        }
    
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
        if (ImGui::BeginChildFrame(ImGui::GetID("ViewportFrame"), ImVec2(0, 0), ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse))
        {
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
                ImTextureID textureID = (ImTextureID)(intptr_t)frameBuffer->getTexture().getRendererID();
                ImVec2 imageSize = ImVec2(
                    static_cast<float>(frameBuffer->getSpecification().width),
                    static_cast<float>(frameBuffer->getSpecification().height)
                );
                ImGui::Image(textureID, imageSize, ImVec2(0, 1), ImVec2(1, 0));
            }

            ImGui::EndChildFrame();
        }
        ImGui::PopStyleVar();
    
        ImGui::End();
    }
    ImGui::PopStyleVar();
}

} // namespace vrm