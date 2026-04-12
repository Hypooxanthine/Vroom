#include "Editor/EntityEditor.h"
#include <Application/Application.h>
#include <Application/GameLayer.h>
#include <Core/Assert.h>
#include <Scene/NameComponent.h>
#include <Scene/Scene.h>
#include <imgui.h>

#include "Core/Profiling.h"
#include "Editor/ComponentEditor.h"

using namespace vrm;

void EntityEditor::ContextualMenuBehaviour(const Entity& e)
{
  bool isRoot = e.isRoot();
  if (isRoot)
    ImGui::BeginDisabled();

  if (ImGui::Selectable("Delete"))
  {
    RequestDeleteEntity(e);
  }

  if (isRoot)
    ImGui::EndDisabled();

  if (ImGui::Selectable("Add child"))
  {
    RequestCreateEntityChild(e);
  }
}

void EntityEditor::RequestDeleteEntity(const Entity& e)
{
  Application::Get().getGameLayer().pushRoutine(Layer::EFrameLocation::ePreEndFrame,
                                                [=](Layer& layer)
                                                {
                                                  auto& gameLayer = static_cast<GameLayer&>(layer);
                                                  gameLayer.getScene().destroyEntity(e);
                                                });
}

void EntityEditor::RequestCreateEntityChild(const Entity& parent)
{
  Application::Get().getGameLayer().pushRoutine(Layer::EFrameLocation::ePreEndFrame,
                                                [e = parent](Layer& layer)
                                                {
                                                  auto&  gameLayer = static_cast<GameLayer&>(layer);
                                                  Entity newChild  = gameLayer.getScene().createEntity();
                                                  gameLayer.getScene().setEntitiesRelation(e, newChild);
                                                });
}

EntityEditor::EntityEditor() : ImGuiElement()
{}

EntityEditor::~EntityEditor()
{}

void EntityEditor::open(const Entity& e)
{
  m_open   = true;
  m_entity = e;
}

void EntityEditor::openOrCloseIfSame(const Entity& e)
{
  if (m_entity == e)
    close();
  else
    open(e);
}

void EntityEditor::close()
{
  m_open   = false;
  m_entity = Entity();
}

bool EntityEditor::isEditingEntity(const Entity& e) const
{
  return m_entity == e;
}

void EntityEditor::onImgui()
{
  VRM_PROFILE_SCOPE("EntityEditor::onImgui");

  if (!m_entity.isValid())
    close();

  if (!m_open)
    return;

  constexpr auto flags = ImGuiWindowFlags_None;

  if (ImGui::Begin("Entity editor", &m_open, flags))
  {
    ComponentEditor::EditEntity(m_entity);
  }
  ImGui::End();

  // User requested close
  if (!m_open)
    close();
}
