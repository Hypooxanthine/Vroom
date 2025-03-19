#include "VroomEditor/UserInterface/Viewport.h"

#include <Vroom/Core/Log.h>
#include <Vroom/Render/Abstraction/FrameBuffer.h>

#include "VroomEditor/UserInterface/UserInterfaceLayer.h"

#include "imgui.h"

using namespace vrm;

Viewport::Viewport()
    : frameBuffer(nullptr), m_LastViewportSize(ImVec2(0.f, 0.f))
{
}

Viewport::~Viewport()
{
}

bool Viewport::onImgui()
{
  bool ret = false;
  m_DidSizeChangeLastFrame = false;

  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
  if (ImGui::Begin("Viewport"))
  {

    if (ImGui::BeginChild("ViewportTopButtons", ImVec2(0, 30)))
    {
      ImGui::SameLine((ImGui::GetWindowWidth() - 150) / 2);

      if (!m_Playing && !m_Simulating)
      {
        m_Playing = ImGui::Button("Play");
        ImGui::SameLine();
        m_Simulating = ImGui::Button("Simulate");
      }
      else
      {
        if (!m_Paused)
          m_Paused = ImGui::Button("Pause");
        else
          m_Paused = !ImGui::Button("Resume");
        ImGui::SameLine();
        bool stopped = ImGui::Button("Stop");
        if (stopped)
        {
          m_Playing = false;
          m_Simulating = false;
          m_Paused = false;
        }
      }
    }
    ImGui::EndChild();

    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
    if (ImGui::BeginChild("ViewportFrame", ImVec2(0, 0), 0, ImGuiWindowFlags_NoScrollbar))
    {
      auto size = ImGui::GetContentRegionAvail();

      m_DidSizeChangeLastFrame = (size.x != m_LastViewportSize.x || size.y != m_LastViewportSize.y);
      m_LastViewportSize = size;

      if (frameBuffer)
      {
        ImTextureID textureID = (ImTextureID)(intptr_t)frameBuffer->getTexture().getRendererID();
        ImVec2 imageSize = ImVec2(
            static_cast<float>(frameBuffer->getSpecification().width),
            static_cast<float>(frameBuffer->getSpecification().height));
        ImGui::Image(textureID, imageSize, ImVec2(0, 1), ImVec2(1, 0));

        m_Active = ImGui::IsWindowFocused() && ImGui::IsMouseDragging(ImGuiMouseButton_Left, 0.f);

        // VRM_LOG_TRACE("Image size: {} {},", imageSize.x, imageSize.y);

        // ImGui::ShowMetricsWindow();
      }
    }
    ImGui::EndChild();
    ImGui::PopStyleVar();
  }
  ImGui::End();
  ImGui::PopStyleVar();

  auto& ui = UserInterfaceLayer::Get();

  if (m_DidSizeChangeLastFrame)
  {
    ui.onViewportResize(m_LastViewportSize.x, m_LastViewportSize.y);
  }

  ui.onViewportActive(m_Active);
  ui.onViewportPaused(m_Paused);
  ui.onViewportSimulating(m_Simulating);

  return ret;
}
