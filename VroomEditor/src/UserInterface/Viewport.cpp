#include "VroomEditor/UserInterface/Viewport.h"

#include <Vroom/Core/Log.h>
#include <Vroom/Core/Application.h>
#include <Vroom/Core/GameLayer.h>
#include <Vroom/Scene/Scene.h>
#include <Vroom/Render/Abstraction/Texture.h>
#include <Vroom/Render/Renderer.h>

#include "VroomEditor/UserInterface/UserInterfaceLayer.h"
#include "VroomEditor/UserInterface/SceneGraph.h"

#include "ImGuizmo.h"

using namespace vrm;

Viewport::Viewport()
  : m_renderTexture(nullptr), m_LastViewportSize(ImVec2(0.f, 0.f))
{
}

Viewport::~Viewport()
{
}

bool Viewport::onImgui()
{
  bool ret = false;
  m_DidSizeChangeLastFrame = false;
  UserInterfaceLayer::ViewportInfos& infos = UserInterfaceLayer::Get().getViewportInfo();

  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
  if (ImGui::Begin("Viewport", m_open))
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
      auto cursor = ImGui::GetCursorPos();
      auto size = ImGui::GetContentRegionAvail();

      m_DidSizeChangeLastFrame = (size.x != m_LastViewportSize.x || size.y != m_LastViewportSize.y);
      m_LastViewportSize = size;

      if (m_renderTexture)
      {
        ImTextureID textureID = (ImTextureID)(intptr_t)m_renderTexture->getRenderId();
        ImVec2 imageSize = ImVec2(
          static_cast<float>(m_renderTexture->getDescription().width),
          static_cast<float>(m_renderTexture->getDescription().height));
        ImGui::Image(textureID, imageSize, ImVec2(0, 1), ImVec2(1, 0));
        ImVec2 rectMin = ImGui::GetItemRectMin();
        ImVec2 rectSize = ImGui::GetItemRectSize();
          infos.clickPos = { -1, -1 };

        if (ImGui::IsMouseHoveringRect(rectMin, ImGui::GetItemRectMax()) && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
        {
          m_clicking = true;
        }
        else if (ImGui::IsMouseDragging(ImGuiMouseButton_Left, 0.01f) || !ImGui::IsWindowFocused() || infos.manipulatingGuizmo)
        {
          m_clicking = false; // Cancels click
        }
        else if (m_clicking && ImGui::IsMouseReleased(ImGuiMouseButton_Left))// && ImGui::GetMouseDragDelta().x == 0.f && ImGui::GetMouseDragDelta().y == 0.f)
        {
          infos.clickPos = glm::ivec2{ (ImGui::GetMousePos() - rectMin).x, rectSize.y - (ImGui::GetMousePos() - rectMin).y };
          m_clicking = false;
        }

        m_Active = ImGui::IsWindowFocused() && ImGui::IsMouseDragging(ImGuiMouseButton_Left, 0.f) && !m_clicking;

        // VRM_LOG_TRACE("Image size: {} {},", imageSize.x, imageSize.y);

        ImGuizmo::SetOrthographic(false);
        ImGuizmo::SetDrawlist(ImGui::GetWindowDrawList());
        ImGuizmo::SetRect(rectMin.x, rectMin.y, rectSize.x, rectSize.y);
      }

      ImGui::PopStyleVar();
      ImGui::SetCursorPos(cursor + ImVec2{5, 5});

      if (ImGui::Button(m_localSpace ? "Local" : "World"))
      {
        m_localSpace = !m_localSpace;
      }
    }
    ImGui::EndChild();
  }
  ImGui::End();
  ImGui::PopStyleVar();

  if ((infos.justChangedSize = m_DidSizeChangeLastFrame))
  {
    infos.width = static_cast<int>(m_LastViewportSize.x);
    infos.height = static_cast<int>(m_LastViewportSize.y);
  }
  infos.active = m_Active;
  infos.paused = m_Paused;
  infos.simulating = m_Simulating;
  infos.playing = m_Playing;
  infos.localSpace = m_localSpace;

  return ret;
}
