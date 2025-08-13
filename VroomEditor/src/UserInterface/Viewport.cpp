#include "VroomEditor/UserInterface/Viewport.h"

#include <Vroom/Core/Log.h>
#include <Vroom/Core/Application.h>
#include <Vroom/Core/GameLayer.h>
#include <Vroom/Scene/Scene.h>
#include <Vroom/Render/Abstraction/Texture.h>
#include <Vroom/Render/Renderer.h>

#include "VroomEditor/UserInterface/UserInterfaceLayer.h"
#include "VroomEditor/UserInterface/EntityEditor.h"
#include "VroomEditor/UserInterface/SceneGraph.h"

#include "imgui.h"

using namespace vrm;

void Viewport::MyViewportModule::onPopupImgui(const ImVec2& texturePx)
{
  if (m_cachedPopupEntity.isValid())
  {
    EntityEditor::ContextualMenuBehaviour(m_cachedPopupEntity);
  }
  else
  {
    bool shouldClose = true;

    if (texturePx.x > -1 && texturePx.y > -1)
    {
      uint32_t rawId = Application::Get().getMainSceneRenderer().getRenderPipeline().getEntityIndexOnPixel({ texturePx.x, texturePx.y });
      auto& scene = Application::Get().getGameLayer().getScene();

      if (scene.entityExists(entt::entity(rawId)))
      {
        m_cachedPopupEntity = scene.getEntity(entt::entity(rawId));
        EntityEditor::ContextualMenuBehaviour(m_cachedPopupEntity);
        VRM_EDITOR_UI_ELEMENT(EntityEditor).open(m_cachedPopupEntity);
        shouldClose = false;
      }
    }

    if (shouldClose)
    {
      m_cachedPopupEntity = {};
      ImGui::CloseCurrentPopup();
    }
  }
}

void Viewport::MyViewportModule::onLeftClick(const ImVec2& texturePx)
{
  if (texturePx.x > -1 && texturePx.y > -1)
  {
    uint32_t rawId = Application::Get().getMainSceneRenderer().getRenderPipeline().getEntityIndexOnPixel({ texturePx.x, texturePx.y });
    UserInterfaceLayer::Get().pushRoutine(Layer::EFrameLocation::ePreEndFrame, [=](Layer& l)
    {
      auto& scene = Application::Get().getGameLayer().getScene();
      auto& sceneGraph = VRM_EDITOR_UI_ELEMENT(SceneGraph);
      auto& entityEditor = VRM_EDITOR_UI_ELEMENT(EntityEditor);

      if (scene.entityExists(entt::entity(rawId)))
      {
        Entity e = scene.getEntity(entt::entity(rawId));
        entityEditor.openOrCloseIfSame(e);
      }
      else
      {
        entityEditor.close();
      }

    });
  }
}

void Viewport::MyViewportModule::onResize(const ImVec2& texturePx)
{
  UserInterfaceLayer::ViewportInfos& infos = UserInterfaceLayer::Get().getViewportInfo();
  infos.justChangedSize = true;
  infos.width = static_cast<int>(texturePx.x);
  infos.height = static_cast<int>(texturePx.y);
}

Viewport::Viewport()
{
  m_viewportModule.setSupportPopup(true);
  m_viewportModule.setSupportGuizmo(true);
}

Viewport::~Viewport()
{
}

void Viewport::onImgui()
{
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

    m_viewportModule.renderImgui();
  }
  ImGui::End();
  ImGui::PopStyleVar();
  
  infos.paused = m_Paused;
  infos.simulating = m_Simulating;
  infos.playing = m_Playing;
  infos.active = m_viewportModule.isDraggingLeft();
  infos.localSpace = m_viewportModule.isLocalSpace();
}
