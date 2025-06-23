#include "VroomEditor/UserInterface/Viewport.h"

#include <Vroom/Core/Log.h>
#include <Vroom/Core/Application.h>
#include <Vroom/Core/GameLayer.h>
#include <Vroom/Scene/Scene.h>
#include <Vroom/Render/Abstraction/Texture2D.h>
#include <Vroom/Render/Renderer.h>

#include "VroomEditor/UserInterface/UserInterfaceLayer.h"
#include "VroomEditor/UserInterface/SceneGraph.h"

#include "ImGuizmo.h"

using namespace vrm;

Viewport::Viewport()
  : renderTexture(nullptr), m_LastViewportSize(ImVec2(0.f, 0.f))
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

      if (renderTexture)
      {
        ImTextureID textureID = (ImTextureID)(intptr_t)renderTexture->getRendererID();
        ImVec2 imageSize = ImVec2(
          static_cast<float>(renderTexture->getWidth()),
          static_cast<float>(renderTexture->getHeight()));
        ImGui::Image(textureID, imageSize, ImVec2(0, 1), ImVec2(1, 0));

        m_Active = ImGui::IsWindowFocused() && ImGui::IsMouseDragging(ImGuiMouseButton_Left, 0.f);
        ImVec2 rectMin = ImGui::GetItemRectMin();
        ImVec2 rectSize = ImGui::GetItemRectSize();

        if (ImGui::IsWindowFocused() && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
        {
          auto px = glm::ivec2{ (ImGui::GetMousePos() - rectMin).x, rectSize.y - (ImGui::GetMousePos() - rectMin).y };
          uint32_t entityId = Renderer::Get().getEntityIndexOnPixel(px);
          VRM_LOG_TRACE("Entity id at pixel ({}, {}): {}", px.x, px.y, entityId);
          auto& sceneGraph = static_cast<SceneGraph&>(UserInterfaceLayer::Get().getElement(EInterfaceElement::eSceneGraph));
          auto entt_e = entt::entity(entityId);

          if (Application::Get().getGameLayer().getScene().getRegistry().valid(entt_e))
          {
            Entity e = Application::Get().getGameLayer().getScene().getEntity(entt_e);
            sceneGraph.selectEntity(e);
          }
        }

        // VRM_LOG_TRACE("Image size: {} {},", imageSize.x, imageSize.y);

        ImGuizmo::SetOrthographic(false);
        ImGuizmo::SetDrawlist(ImGui::GetWindowDrawList());
        ImGuizmo::SetRect(rectMin.x, rectMin.y, rectSize.x, rectSize.y);
      }

      ImGui::PopStyleVar(2);
      ImGui::SetCursorPos(cursor + ImVec2{5, 5});

      if (ImGui::Button(m_localSpace ? "Local" : "World"))
      {
        m_localSpace = !m_localSpace;
      }
    }
    ImGui::EndChild();
  }
  ImGui::End();

  UserInterfaceLayer::ViewportInfos& infos = UserInterfaceLayer::Get().getViewportInfo();

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
