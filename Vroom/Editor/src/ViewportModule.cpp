#include "Editor/ViewportModule.h"
#include "ImGuizmo.h"
#include "Rasterizer/Texture.h"
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

      if (m_supportPopup && ImGui::BeginPopupContextItem("popup"))
      {
        onPopupImgui(texturePos);
        ImGui::EndPopup();
      }

      bool overGizmoUi = false;
      if (m_supportGuizmo)
      {
        overGizmoUi = ImGuizmo::IsOver() || ImGuizmo::IsUsing();

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
        overGizmoUi = overGizmoUi || ImGui::IsItemHovered();
      }

      handleMouseInput(texturePos, overGizmoUi);
    }

    ImGui::PopStyleVar();
  }
  ImGui::EndChild();
}

void ViewportModule::handleMouseInput(const ImVec2& texturePx, bool overGizmoUi)
{
  const bool surfaceHovered = ImGui::IsWindowHovered() && !overGizmoUi;

  m_lmbDragging = false;

  if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
  {
    m_pressedOnViewport = surfaceHovered;
    m_clicking          = surfaceHovered;
  }
  else if (m_pressedOnViewport && ImGui::IsMouseDown(ImGuiMouseButton_Left))
  {
    if (m_clicking && ImGui::IsMouseDragging(ImGuiMouseButton_Left, s_clickDragThreshold))
      m_clicking = false;

    if (!m_clicking)
    {
      m_lmbDragging = true;
      onDrag(ImGuiMouseButton_Left, texturePx);
    }
  }
  else if (ImGui::IsMouseReleased(ImGuiMouseButton_Left))
  {
    if (m_clicking && surfaceHovered)
      onLeftClick(texturePx);

    m_clicking          = false;
    m_pressedOnViewport = false;
  }
}

void ViewportModule::closePopup()
{
  ImGui::CloseCurrentPopup();
}
