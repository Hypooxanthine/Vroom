#include "VroomEditor/UserInterface/ViewportModule.h"
#include "ImGuizmo.h"
#include "Vroom/Render/Abstraction/Texture.h"
#include "imgui.h"

using namespace vrm;


ViewportModule::ViewportModule()
{

}

ViewportModule::~ViewportModule()
{

}

void ViewportModule::setSupportPopup(bool val)
{
  m_supportPopup = val;
}

void ViewportModule::setSupportGuizmo(bool val, const render::NormalizedViewport& viewport)
{
  m_supportGuizmo = val;
  m_guizmoViewport = viewport;
}

void ViewportModule::allowDraggingIfMousePressed()
{
  // Clicking state is preventing dragging to start
  m_clicking = false;
}

void ViewportModule::onImgui()
{
  ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
  if (ImGui::BeginChild("ViewportFrame", ImVec2(0, 0), 0, ImGuiWindowFlags_NoScrollbar))
  {
    auto cursor = ImGui::GetCursorPos();
    auto size = ImGui::GetContentRegionAvail();

    if (size != m_size)
    {
      m_size = size;
      onResize(size);
    }

    if (m_texture)
    {
      ImTextureID textureID = (ImTextureID)(intptr_t)m_texture->getRenderId();
      ImVec2 imageSize = ImVec2(
        static_cast<float>(m_texture->getDescription().width),
        static_cast<float>(m_texture->getDescription().height));
      ImGui::Image(textureID, imageSize, ImVec2(0, 1), ImVec2(1, 0));
      ImVec2 rectMin = ImGui::GetItemRectMin();
      ImVec2 rectSize = ImGui::GetItemRectSize();

      ImVec2 texturePos = { (ImGui::GetMousePos() - rectMin).x, rectSize.y - (ImGui::GetMousePos() - rectMin).y };

      if (m_supportPopup && !ImGui::IsMouseDragging(ImGuiMouseButton_Left, 0.01f) && !m_clicking && ImGui::BeginPopupContextItem("popup"))
      {
        onPopupImgui(texturePos);
        ImGui::EndPopup();
      }

      if (!ImGui::IsPopupOpen("popup"))
      {
        if (ImGui::IsMouseHoveringRect(rectMin, ImGui::GetItemRectMax()) && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
        {
          m_clicking = true;
        }
        else if (ImGui::IsMouseDragging(ImGuiMouseButton_Left, 0.01f) || !ImGui::IsWindowFocused())
        {
          m_clicking = false; // Cancels click
        }
        else if (m_clicking && ImGui::IsMouseReleased(ImGuiMouseButton_Left))
        {
          onLeftClick(texturePos);
          m_clicking = false;
        }

        m_lmbDragging = false;

        if (ImGui::IsWindowFocused() && !m_clicking)
        {
          if (ImGui::IsMouseDragging(ImGuiMouseButton_Left, 0.f))
          {
            m_lmbDragging = true;
            onDrag(ImGuiMouseButton_Left, texturePos);
          }
        }
      }

      if (m_supportGuizmo)
      {
        ImGuizmo::SetOrthographic(false);
        ImGuizmo::SetDrawlist(ImGui::GetWindowDrawList());
        const glm::vec2& o = m_guizmoViewport.getOrigin();
        const glm::vec2& s = m_guizmoViewport.getSize();
        ImGuizmo::SetRect(rectMin.x + rectSize.x * o.x, rectMin.y + rectSize.y * o.y, rectSize.x * s.x, rectSize.y * s.y);

        ImGui::SetCursorPos(cursor + ImVec2{ 5, 5 });
        if (ImGui::Button(m_localSpace ? "Local" : "World"))
        {
          m_localSpace = !m_localSpace;
        }
      }
    }

    ImGui::PopStyleVar();
  }
  ImGui::EndChild();
}

void ViewportModule::closePopup()
{
  ImGui::CloseCurrentPopup();
}
